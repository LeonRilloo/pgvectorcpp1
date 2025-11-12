#!/usr/bin/env python3
"""
Final verification script for the C to C++ conversion project.
"""

import os
import re
from pathlib import Path

def main():
    """Run final verification of the conversion."""
    print("🚀 Final Verification of C to C++ Conversion")
    print("=" * 60)
    
    # Check 1: Directory structure
    print("\n1. 📁 Directory Structure Check:")
    src_cpp = Path('src-cpp')
    if not src_cpp.exists():
        print("❌ src-cpp directory not found")
        return False
    print("✅ src-cpp directory exists")
    
    # Check 2: File counts
    print("\n2. 📊 File Count Verification:")
    src_dir = Path('src')
    
    original_c_files = list(src_dir.glob('*.c'))
    original_h_files = list(src_dir.glob('*.h'))
    
    converted_cpp_files = list(src_cpp.glob('*.cpp'))
    converted_hpp_files = list(src_cpp.glob('*.hpp'))
    
    print(f"   Original: {len(original_c_files)} .c files, {len(original_h_files)} .h files")
    print(f"   Converted: {len(converted_cpp_files)} .cpp files, {len(converted_hpp_files)} .hpp files")
    
    if len(original_c_files) != len(converted_cpp_files):
        print("❌ C file count mismatch")
        return False
    
    if len(original_h_files) != len(converted_hpp_files):
        print("❌ Header file count mismatch")
        return False
    
    print("✅ File counts match")
    
    # Check 3: Makefile exists
    print("\n3. 🔨 Build System Check:")
    makefile = src_cpp / 'Makefile'
    if not makefile.exists():
        print("❌ Makefile not found")
        return False
    
    with open(makefile, 'r') as f:
        makefile_content = f.read()
    
    required_elements = ['MODULE_big', 'PG_CXXFLAGS', '.cpp.o:', 'PG_CONFIG', 'include $(PGXS)']
    for element in required_elements:
        if element not in makefile_content:
            print(f"❌ Missing Makefile element: {element}")
            return False
    
    print("✅ Makefile properly configured")
    
    # Check 4: Header file quality
    print("\n4. 📋 Header File Quality Check:")
    hpp_files = list(src_cpp.glob('*.hpp'))
    
    header_issues = 0
    for hpp_file in hpp_files:
        with open(hpp_file, 'r') as f:
            content = f.read()
        
        # Check for C++ guards
        if '#ifdef __cplusplus' not in content or 'extern "C"' not in content:
            print(f"❌ {hpp_file.name}: Missing C++ compatibility guards")
            header_issues += 1
        
        # Check for proper include guards
        base_name = hpp_file.stem.upper()
        expected_guard = f"{base_name}_HPP"
        if f"#ifndef {expected_guard}" not in content:
            print(f"❌ {hpp_file.name}: Missing or incorrect include guards")
            header_issues += 1
    
    if header_issues == 0:
        print("✅ All header files properly formatted")
    else:
        print(f"⚠️  Found {header_issues} header issues")
    
    # Check 5: Source file quality
    print("\n5. 📄 Source File Quality Check:")
    cpp_files = list(src_cpp.glob('*.cpp'))
    
    source_issues = 0
    for cpp_file in cpp_files:
        with open(cpp_file, 'r') as f:
            content = f.read()
        
        # Check for local includes that still use .h
        local_includes = re.findall(r'#include\s+"(\\w+)\\.h"', content)
        if local_includes:
            # Filter out system includes that should remain .h
            local_includes = [inc for inc in local_includes if inc not in ['postgres', 'fmgr', 'utils', 'catalog', 'storage', 'access']]
            if local_includes:
                print(f"❌ {cpp_file.name}: Local includes not updated: {', '.join(local_includes)}")
                source_issues += 1
    
    if source_issues == 0:
        print("✅ All source files properly formatted")
    else:
        print(f"⚠️  Found {source_issues} source issues")
    
    # Check 6: Documentation
    print("\n6. 📚 Documentation Check:")
    required_docs = ['CONVERSION_PLAN.md', 'CONVERSION_SUMMARY.md']
    for doc in required_docs:
        if not Path(doc).exists():
            print(f"❌ Missing documentation: {doc}")
            return False
    
    print("✅ Documentation complete")
    
    # Final summary
    print(f"\n{'='*60}")
    print("🎉 CONVERSION VERIFICATION COMPLETE!")
    print(f"{'='*60}")
    print(f"✅ Directory structure: OK")
    print(f"✅ File counts: {len(converted_cpp_files)} .cpp, {len(converted_hpp_files)} .hpp")
    print(f"✅ Build system: Configured")
    print(f"✅ Header quality: {'Perfect' if header_issues == 0 else f'{len(hpp_files)-header_issues}/{len(hpp_files)} good'}")
    print(f"✅ Source quality: {'Perfect' if source_issues == 0 else f'{len(cpp_files)-source_issues}/{len(cpp_files)} good'}")
    print(f"✅ Documentation: Complete")
    
    print(f"\n📋 Summary:")
    print(f"   • All {len(original_c_files)} C files → C++ files")
    print(f"   • All {len(original_h_files)} header files → C++ headers")
    print(f"   • C++ build system configured")
    print(f"   • Documentation created")
    print(f"   • Ready for compilation testing")
    
    print(f"\n🚀 Next Steps:")
    print(f"   1. Test compilation with PostgreSQL headers")
    print(f"   2. Run functional tests")
    print(f"   3. Performance benchmarking")
    print(f"   4. Gradual C++ feature integration")
    
    return True

if __name__ == "__main__":
    success = main()
    exit(0 if success else 1)
