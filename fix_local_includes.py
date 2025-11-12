#!/usr/bin/env python3
"""
Fix local include statements in C++ files.
"""

import re
from pathlib import Path

def fix_local_includes():
    """Fix local include statements."""
    dst_dir = Path('src-cpp')
    cpp_files = list(dst_dir.glob('*.cpp'))
    
    # Local headers that should be converted to .hpp
    local_headers = ['bitutils', 'bitvec', 'halfutils', 'halfvec', 'hnsw', 'ivfflat', 'sparsevec', 'vector']
    
    for cpp_file in cpp_files:
        print(f"Processing {cpp_file.name}...")
        
        with open(cpp_file, 'r') as f:
            content = f.read()
        
        original_content = content
        
        # Fix local includes
        for header in local_headers:
            content = re.sub(f'#include "{header}.h"', f'#include "{header}.hpp"', content)
        
        if content != original_content:
            with open(cpp_file, 'w') as f:
                f.write(content)
            print(f"  ✅ Updated local includes in {cpp_file.name}")
        else:
            print(f"  ℹ️  No changes needed")

if __name__ == "__main__":
    fix_local_includes()
