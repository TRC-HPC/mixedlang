from mixedlang import *

def add(i : int, j : int) -> int:
    ml_logger.info(f'Calculating sum of {i} + {j}')
    return i + j
