#!/usr/bin/env python3

import os
import sys

from pathlib import Path
root_path = Path(os.path.dirname(os.path.abspath(__file__))).resolve()
sys.path.append(os.path.join(root_path, 'lib'))
sys.path.append(os.path.join(root_path, 'modules'))

from pymixedlang import get_config
from mixedlang import *

if __name__ == '__main__':
    cfg = get_config()
    print(f'Config: {cfg}')

    ml_logger.info('The logger is working!')