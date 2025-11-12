#!/usr/bin/env python3
"""
Script to convert C code to C++ for the pgvector project.
This script handles the systematic conversion of .c and .h files to .cpp and .hpp files.
"""

import os
import re
import shutil
from pathlib import Path

def convert_header_file(content, filename):
    """Convert C header file to C++ header file with proper guards and namespaces."""
    # Convert include guards
    base_name = filename.replace('.hpp', '').upper()
    new_guard = f"{base_name}_HPP"
    
    # Replace #ifndef/#define/#endif guards
    content = re.sub(r'#ifndef\s+\w+_H\s*\n#define\s+\w+_H', 
                     f'#ifndef {new_guard}\n#define {new_guard}', content)
    content = re.sub(r'#endif\s*/\*\s*\w+_H\s*\*/', f'#endif /* {new_guard} */', content)
    
    # Add C++ guards for C compatibility
    cpp_content = f'''#ifdef __cplusplus
extern "C" {{
#endif

{content}

#ifdef __cplusplus
}}
#endif
'''
    
    return cpp_content

def convert_source_file(content, filename):
    """Convert C source file to C++ source file."""
    # Change .h includes to .hpp
    content = re.sub(r'#include\s+"(\\w+)\\.h"', r'#include "\1.hpp"', content)
    
    # Add C++ compatibility for C headers
    cpp_headers = '''#ifdef __cplusplus
extern "C" {
#endif

#include "postgres.h"

#ifdef __cplusplus
}
#endif
'''
    
    # Replace postgres.h include with C++ compatible version
    content = re.sub(r'#include\s+"postgres.h"', cpp_headers, content)
    
    return content

def convert_files():
    """Convert all C files in src directory to C++ files in src-cpp directory."""
    src_dir = Path('src')
    dst_dir = Path('src-cpp')
    
    if not src_dir.exists():
        print("Error: src directory not found")
        return
    
    # Create destination directory if it doesn't exist
    dst_dir.mkdir(exist_ok=True)
    
    # Process all .c and .h files
    for src_file in src_dir.glob('*'):
        if src_file.suffix not in ['.c', '.h']:
            continue
            
        # Determine destination filename
        if src_file.suffix == '.c':
            dst_filename = src_file.stem + '.cpp'
        else:
            dst_filename = src_file.stem + '.hpp'
        
        dst_file = dst_dir / dst_filename
        
        print(f"Converting {src_file} -> {dst_file}")
        
        # Read source file
        with open(src_file, 'r') as f:
            content = f.read()
        
        # Convert content
        if src_file.suffix == '.h':
            converted_content = convert_header_file(content, dst_filename)
        else:
            converted_content = convert_source_file(content, dst_filename)
        
        # Write destination file
        with open(dst_file, 'w') as f:
            f.write(converted_content)
    
    print(f"Conversion complete! Files created in {dst_dir}")

if __name__ == "__main__":
    convert_files()
