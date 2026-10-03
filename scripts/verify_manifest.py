#!/usr/bin/env python3
from pathlib import Path
import csv, hashlib, sys

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "release" / "FINAL_RELEASE_MANIFEST_SHA256.csv"

def sha256_file(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for chunk in iter(lambda:f.read(1024*1024), b""):
            h.update(chunk)
    return h.hexdigest()

if not MANIFEST.exists():
    print("FAIL: manifest not found:", MANIFEST)
    sys.exit(2)

rows = list(csv.DictReader(MANIFEST.open("r", encoding="utf-8-sig", newline="")))
bad = []

for r in rows:
    p = ROOT / r["path"]
    if not p.exists():
        bad.append((r["path"], "missing"))
        continue
    if p.stat().st_size != int(r["bytes"]):
        bad.append((r["path"], "size"))
        continue
    if sha256_file(p) != r["sha256"]:
        bad.append((r["path"], "sha256"))

if bad:
    print("FAIL")
    for x in bad[:50]:
        print(" -", x)
    sys.exit(1)

print(f"PASS: {len(rows)}/{len(rows)} manifest entries verified.")
