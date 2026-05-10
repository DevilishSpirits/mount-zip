#!/bin/python3

# Copyright 2026 Google LLC
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import os
import os.path
from zipfile import ZipFile

dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'data')
tmp = os.path.join(dir, 'many_nodes.zip~')

try:
  with ZipFile(tmp, 'w') as z:
    for i in range(100):
      print('\rWriting many_nodes.zip... %3d %%' % i, end='', flush=True)
      for j in range(100):
        z.writestr('%02d/%02d' % (i, j) + '/x' * 2000, b'File %02d/%02d\n' % (i, j))

  print('\r\033[2KDone', flush=True)
  os.replace(tmp, os.path.join(dir, 'many_nodes.zip'))
except:
  os.remove(tmp)
