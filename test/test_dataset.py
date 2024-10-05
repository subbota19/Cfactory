import unittest
import os
import sys

TEST_DIR = os.path.dirname(__file__)
CPYTHON_PATH = "cfactorypython"

sys.path.append(os.path.abspath(os.path.join(TEST_DIR, '..', CPYTHON_PATH)))

from _dataset import PyDataset, PyRow


class TestPyDataset(unittest.TestCase):
    def setUp(self):
        self.schema = [
            ("id", "int"),
            ("value", "float"),
            ("name", "string"),
        ]
        self.dataset = PyDataset(schema=self.schema)

    def test_add_and_get_row(self):
        row = PyRow(fields={"id": 1, "value": 3.14, "name": "Alice"})
        self.dataset.add_row(row)
        # inserted_row = self.dataset.get_row(0)
        self.dataset.print_row(0)
        # self.assertEqual(inserted_row, {"id": 1, "value": 3.14, "name": "Alice"})


if __name__ == '__main__':
    unittest.main()
