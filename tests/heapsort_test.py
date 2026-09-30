import os
import subprocess
import unittest

from pathlib import Path

RAIZ = Path(__file__).resolve().parent.parent
EXECUTABLE = RAIZ / 'bin/heapsort'

class TestHeapsort(unittest.TestCase):
    def test_executable_exists(self):
        self.assertEqual(os.path.exists(EXECUTABLE), True)

    def test_cases(self):
        casos = {
            "vazio": [],
            "unitario": [42],
            "exemplo": [4, -2, 7, 4, 0, -8, 3],
            "extremos": [-(2**31), 2**31 - 1, 0, -(2**31), 2**31 - 1],
            "crescente_8000": list(range(8000)),
            "decrescente_8000": list(range(8000, 0, -1)),
            "iguais_8000": [7] * 8000,
            "duplicados_8000": [i % 11 - 5 for i in range(8000)],
        }
        for name, values in casos.items():
            with self.subTest(name=name):
                entrada = f"{len(values)}\n" + " ".join(map(str, values)) + "\n"
                process = subprocess.run(
                    [EXECUTABLE],
                    input=entrada,
                    text=True,
                    capture_output=True,
                    check=False,
                    timeout=5,
                )
                expected = sorted(values)
                self.assertEqual(0, process.returncode, process.stderr)
                self.assertListEqual(expected, list(map(int, process.stdout.split())))


if __name__ == '__main__':
    unittest.main()
