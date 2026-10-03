#!/usr/bin/env python3

from pathlib import Path
import csv
import hashlib
import sys


ROOT = Path(__file__).resolve().parents[1]

MANIFEST_REL = "release/FINAL_RELEASE_MANIFEST_SHA256.csv"
MANIFEST = ROOT / MANIFEST_REL

EXPECTED_COLUMNS = {"path", "bytes", "sha256"}

REQUIRED_FILES = {
    "README.md",
    "CITATION.cff",
    "LICENSE",
    "REPRODUCE.md",
    "release/FINAL_PUBLIC_RELEASE_STATUS.json",
    "scripts/verify_manifest.py",
}


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()

    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)

    return h.hexdigest()


def fail(message: str, code: int = 1):
    print(f"FAIL: {message}")
    sys.exit(code)


# ------------------------------------------------------------
# 1. Manifest must exist
# ------------------------------------------------------------

if not MANIFEST.exists():
    fail(f"manifest not found: {MANIFEST}", 2)


# ------------------------------------------------------------
# 2. Read and validate manifest structure
# ------------------------------------------------------------

try:
    with MANIFEST.open(
        "r",
        encoding="utf-8-sig",
        newline=""
    ) as f:
        reader = csv.DictReader(f)

        if reader.fieldnames is None:
            fail("manifest has no header")

        fields = set(reader.fieldnames)

        if not EXPECTED_COLUMNS.issubset(fields):
            fail(
                "manifest columns invalid. "
                f"Required: {sorted(EXPECTED_COLUMNS)}, "
                f"found: {reader.fieldnames}"
            )

        rows = list(reader)

except Exception as exc:
    fail(f"could not read manifest: {exc}")


# ------------------------------------------------------------
# 3. Empty manifest must NEVER pass
# ------------------------------------------------------------

if len(rows) == 0:
    fail("manifest contains zero entries")


# ------------------------------------------------------------
# 4. Validate manifest paths and detect duplicates
# ------------------------------------------------------------

manifest_paths = []
normalized_rows = []

for index, row in enumerate(rows, start=2):

    raw_path = str(row.get("path", "")).strip()

    if not raw_path:
        fail(f"empty path at manifest line {index}")

    # Normalize Windows separators to repository-style paths.
    rel = raw_path.replace("\\", "/")

    path_obj = Path(rel)

    if path_obj.is_absolute():
        fail(f"absolute path is not allowed: {raw_path}")

    if ".." in path_obj.parts:
        fail(f"parent traversal is not allowed: {raw_path}")

    # The manifest never hashes itself.
    if rel == MANIFEST_REL:
        fail(
            f"manifest must not list itself: {MANIFEST_REL}"
        )

    manifest_paths.append(rel)
    normalized_rows.append((rel, row))


if len(manifest_paths) != len(set(manifest_paths)):
    duplicates = sorted(
        {
            p
            for p in manifest_paths
            if manifest_paths.count(p) > 1
        }
    )

    fail(
        "duplicate manifest paths detected: "
        + ", ".join(duplicates[:20])
    )


manifest_set = set(manifest_paths)


# ------------------------------------------------------------
# 5. Required public-release files must be represented
# ------------------------------------------------------------

missing_required_from_manifest = sorted(
    REQUIRED_FILES - manifest_set
)

if missing_required_from_manifest:
    fail(
        "required files missing from manifest: "
        + ", ".join(missing_required_from_manifest)
    )


# ------------------------------------------------------------
# 6. Build the ACTUAL repository file set
#
# Exclusions:
#   - .git internal metadata
#   - manifest itself
# ------------------------------------------------------------

actual_set = set()

for p in ROOT.rglob("*"):

    if not p.is_file():
        continue

    rel = p.relative_to(ROOT).as_posix()

    if ".git" in p.relative_to(ROOT).parts:
        continue

    if rel == MANIFEST_REL:
        continue

    actual_set.add(rel)


# ------------------------------------------------------------
# 7. Exact file-set equality
#
# Detect:
#   - file listed in manifest but absent from repo
#   - file present in repo but absent from manifest
# ------------------------------------------------------------

missing_files = sorted(manifest_set - actual_set)
extra_files = sorted(actual_set - manifest_set)


if missing_files:
    print("FAIL: files listed in manifest are missing:")

    for path in missing_files[:50]:
        print(" -", path)

    sys.exit(1)


if extra_files:
    print("FAIL: repository contains files not listed in manifest:")

    for path in extra_files[:50]:
        print(" -", path)

    sys.exit(1)


# ------------------------------------------------------------
# 8. Size + SHA256 verification
# ------------------------------------------------------------

bad = []

for rel, row in normalized_rows:

    path = ROOT / rel

    try:
        expected_size = int(row["bytes"])
    except Exception:
        bad.append((rel, "invalid bytes value"))
        continue

    expected_sha = str(row["sha256"]).strip().lower()

    if len(expected_sha) != 64:
        bad.append((rel, "invalid SHA256 value"))
        continue

    actual_size = path.stat().st_size

    if actual_size != expected_size:
        bad.append(
            (
                rel,
                f"size mismatch "
                f"(expected {expected_size}, got {actual_size})",
            )
        )
        continue

    actual_sha = sha256_file(path)

    if actual_sha.lower() != expected_sha:
        bad.append(
            (
                rel,
                f"SHA256 mismatch "
                f"(expected {expected_sha}, got {actual_sha})",
            )
        )


if bad:
    print("FAIL: integrity errors detected:")

    for path, reason in bad[:50]:
        print(f" - {path}: {reason}")

    sys.exit(1)


# ------------------------------------------------------------
# 9. Final PASS
# ------------------------------------------------------------

print(
    f"PASS: {len(rows)}/{len(rows)} manifest entries verified; "
    f"exact repository file set matched; "
    f"{len(REQUIRED_FILES)}/{len(REQUIRED_FILES)} required files present."
)

sys.exit(0)