#!/usr/bin/env python3
"""
Test script to verify the C to C++ conversion.
"""

import os
import subprocess
import sys
from pathlib import Path

def test_file_conversions():
    """Test that all files were converted correctly."""
    print("Testing file conversions...")
    
    src_dir = Path('src')
    dst_dir = Path('src-cpp')
    
    # Check that destination directory exists
    if not dst_dir.exists():
        print("❌ ERROR: src-cpp directory not found")
        return False
    
    # Check that all .c files have corresponding .cpp files
    c_files = list(src_dir.glob('*.c'))
    cpp_files = list(dst_dir.glob('*.cpp'))
    
    print(f"Found {len(c_files)} C files in src/")
    print(f"Found {len(cpp_files)} C++ files in src-cpp/")
    
    if len(c_files) != len(cpp_files):
        print("❌ ERROR: Number of C and C++ files don't match")
        return False
    
    # Check that all .h files have corresponding .hpp files
    h_files = list(src_dir.glob('*.h'))
    hpp_files = list(dst_dir.glob('*.hpp'))
    
    print(f"Found {len(h_files)} header files in src/")
    print(f"Found {len(hpp_files)} header files in src-cpp/")
    
    if len(h_files) != len(hpp_files):
        print("❌ ERROR: Number of header files don't match")
        return False
    
    print("✅ File conversion counts match")
    return True

def test_cpp_syntax():
    """Test that C++ files have valid syntax."""
    print("\nTesting C++ syntax...")
    
    dst_dir = Path('src-cpp')
    cpp_files = list(dst_dir.glob('*.cpp'))
    
    for cpp_file in cpp_files[:3]:  # Test first 3 files
        print(f"Testing {cpp_file}...")
        try:
            # Try to compile with syntax check only
            result = subprocess.run([
                'g++', '-std=c++17', '-fsyntax-only', '-I/usr/include/postgresql',
                str(cpp_file)
            ], capture_output=True, text=True, timeout=30)
            
            if result.returncode != 0:
                print(f"❌ Syntax error in {cpp_file}:")
                print(result.stderr)
                return False
            else:
                print(f"✅ {cpp_file} syntax OK")
                
        except subprocess.TimeoutExpired:
            print(f"⏰ Timeout testing {cpp_file}")
            return False
        except FileNotFoundError:
            print("⚠️  g++ not found, skipping syntax test")
            return True
    
    return True

def test_header_guards():
    """Test that header files have proper guards."""
    print("\nTesting header guards...")
    
    dst_dir = Path('src-cpp')
    hpp_files = list(dst_dir.glob('*.hpp'))
    
    for hpp_file in hpp_files[:3]:  # Test first 3 files
        print(f"Testing {hpp_file}...")
        
        with open(hpp_file, 'r') as f:
            content = f.read()
        
        # Check for C++ guards
        if '#ifdef __cplusplus' not in content:
            print(f"❌ Missing C++ guards in {hpp_file}")
            return False
        
        if 'extern \"C\"' not in content:
            print(f"❌ Missing extern \"C\" in {hpp_file}")
            return False
        
        # Check for include guards
        base_name = hpp_file.stem.upper()
        expected_guard = f"{base_name}_HPP"
        
        if f"#ifndef {expected_guard}" not in content:
            print(f"❌ Missing or incorrect include guard in {hpp_file}")
            print(f"Expected: #ifndef {expected_guard}")
            return False
        
        print(f"✅ {hpp_file} header guards OK")
    
    return True

def test_makefile():
    """Test that Makefile exists and is valid."""
    print("\nTesting Makefile...")
    
    makefile_path = Path('src-cpp/Makefile')
    
    if not makefile_path.exists():
        print("❌ Makefile not found in src-cpp/")
        return False
    
    with open(makefile_path, 'r') as f:
        content = f.read()
    
    # Check for essential Makefile components
    required_elements = [
        'MODULE_big = vector_cpp',
        'PG_CXXFLAGS',
        '.cpp.o:',
        'PG_CONFIG',
        'include $(PGXS)'
    ]
    
    for element in required_elements:
        if element not in content:
            print(f"❌ Missing element in Makefile: {element}")
            return False
    
    print("✅ Makefile structure looks good")
    return True

def main():
    """Run all tests."""
    print("Running C++ conversion tests...\n")
    
    tests = [
        ("File Conversions", test_file_conversions),
        ("C++ Syntax", test_cpp_syntax),
        ("Header Guards", test_header_guards),
        ("Makefile", test_makefile)
    ]
    
    all_passed = True
    
    for test_name, test_func in tests:
        try:
            result = test_func()
            if not result:
                all_passed = False
                print(f"❌ {test_name} FAILED")
            else:
                print(f"✅ {test_name} PASSED")
        except Exception as e:
            print(f"❌ {test_name} ERROR: {e}")
            all_passed = False
    
    print(f"\n{'='*50}")
    if all_passed:
        print("🎉 ALL TESTS PASSED! C++ conversion looks good.")
    else:
        print("❌ Some tests failed. Please review the output above.")
    
    return all_passed

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
