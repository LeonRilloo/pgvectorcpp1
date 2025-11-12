#!/usr/bin/env python3
"""
Script to fix include statements in converted C++ files.
"""

import os
import re
from pathlib import Path

def fix_includes_in_file(filepath):
    """Fix include statements in a C++ file."""
    with open(filepath, 'r') as f:
        content = f.read()
    
    original_content = content
    
    # Fix local includes (change .h to .hpp)
    content = re.sub(r'#include\s+"(\\w+)\\.h"', r'#include "\1.hpp"', content)
    
    # Fix system includes (they should remain as .h)
    # Don't change PostgreSQL system includes
    
    if content != original_content:
        with open(filepath, 'r') as f:
            original_lines = f.readlines()
        
        with open(filepath, 'w') as f:
            for line in original_lines:
                # Only change local includes, not system includes
                if '#include "' in line and not '#include <' in line:
                    line = re.sub(r'#include\s+"(\\w+)\\.h"', r'#include "\1.hpp"', line)
                f.write(line)
        
        return True
    
    return False

def main():
    """Fix includes in all C++ files."""
    dst_dir = Path('src-cpp')
    cpp_files = list(dst_dir.glob('*.cpp'))
    
    print(f"Fixing includes in {len(cpp_files)} C++ files...")
    
    fixed_count = 0
    for cpp_file in cpp_files:
        if fix_includes_in_file(cpp_file):
            print(f"✅ Fixed includes in {cpp_file.name}")
            fixed_count += 1
        else:
            print(f"ℹ️  No changes needed in {cpp_file.name}")
    
    print(f"\nFixed includes in {fixed_count} files.")

if __name__ == "__main__":
    main()
