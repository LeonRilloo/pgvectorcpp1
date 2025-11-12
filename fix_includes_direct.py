#!/usr/bin/env python3
"""
Direct fix for include statements in C++ files.
"""

import re
from pathlib import Path

def fix_includes():
    """Fix include statements in all C++ files."""
    dst_dir = Path('src-cpp')
    cpp_files = list(dst_dir.glob('*.cpp'))
    
    for cpp_file in cpp_files:
        print(f"Processing {cpp_file.name}...")
        
        with open(cpp_file, 'r') as f:
            lines = f.readlines()
        
        modified = False
        new_lines = []
        
        for line in lines:
            original_line = line
            
            # Fix local includes (change .h to .hpp) but not system includes
            if '#include "' in line and not any(sys_inc in line for sys_inc in ['postgres.h', 'catalog/', 'common/', 'fmgr.h', 'utils/', 'storage/', 'access/', 'commands/', 'executor/', 'nodes/', 'optimizer/', 'parser/', 'tcop/', 'utils/']):
                # This is a local include, change .h to .hpp
                line = re.sub(r'#include\s+"(\\w+)\\.h"', r'#include "\1.hpp"', line)
            
            if line != original_line:
                modified = True
                print(f"  Fixed: {original_line.strip()} -> {line.strip()}")
            
            new_lines.append(line)
        
        if modified:
            with open(cpp_file, 'w') as f:
                f.writelines(new_lines)
            print(f"  ✅ Updated {cpp_file.name}")
        else:
            print(f"  ℹ️  No changes needed")

if __name__ == "__main__":
    fix_includes()
