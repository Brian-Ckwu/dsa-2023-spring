import sys
from pathlib import Path

if __name__ == "__main__":
    a = Path(sys.argv[1]).read_text().split()
    b = Path(sys.argv[2]).read_text().split()
    assert len(a) == len(b)
    for i in range(len(a)):
        print(f"Line {i}: ok")
        if a[i] != b[i]:
            print(f"Line {i}: {a[i]} != {b[i]}")
            break;