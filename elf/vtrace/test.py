#!/usr/bin/env python3


import os
import subprocess
import tempfile

CWD = os.getcwd()

LIBASAN = (
    subprocess.check_output(["gcc", "-print-file-name=libasan.so"])
    .decode("utf-8")
    .strip()
)
LIBVTRACE = os.path.join(CWD, "libvtrace.so")


def test_full_output(test: str, expected_output: str):
    with tempfile.TemporaryDirectory() as temp_dir:
        path = os.path.join(CWD, test)
        _ = os.system(f"cp {path} {temp_dir}/test")
        output = subprocess.check_output(
            [f"{temp_dir}/test"],
            env={
                "LD_PRELOAD": f"{LIBASAN}:{LIBVTRACE}",
                "ASAN_OPTIONS": "detect_leaks=1",
            },
        )

    assert (
        output.decode("utf-8") == expected_output
    ), f"test {test} failed: expected {expected_output}, got {output.decode('utf-8')}"
    print(f"test {test} passed")


if __name__ == "__main__":
    test_full_output(
        "hello-world",
        "[puts]\nHello, world!\n",
    )
    test_full_output(
        "local",
        "[puts]\nlol\n[puts]\nkek\n[puts]\nkek\n[puts]\nlol\n[printf]\n3\n",
    )
    test_full_output(
        "big",
        "[malloc]\n[memset]\n[puts]\naaaaaaaaa\n[free]\n" * 1000,
    )
    test_full_output(
        "custom",
        "[sum]\n[printf]\n3\n[average]\n[printf]\n3.000000\n",
    )
    test_full_output(
        "recoursive",
        "[foo]\n" + "foo\nbar\n" * 5 + "[bar]\n" + "bar\nfoo\n" * 5,
    )
    test_full_output(
        "full-relro",
        "[puts]\nI am running with full RELRO\n"  # pyright: ignore[reportImplicitStringConcatenation]
        "[puts]\nThat means that GOT should be read-only\n"
        "[puts]\nHe-he\n",
    )
