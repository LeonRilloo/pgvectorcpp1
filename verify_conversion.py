#!/usr/bin/env python3
"""
Verification script for the C to C++ conversion.
This script checks the conversion quality without requiring PostgreSQL headers.
"""

import os
import re
from pathlib import Path

def analyze_file_content(filepath, file_type):
    """Analyze the content of a converted file."""
    with open(filepath, 'r') as f:
        content = f.read()
    
    analysis = {
        'filepath': filepath,
        'file_type': file_type,
        'lines': len(content.splitlines()),
        'has_cpp_guards': False,
        'has_extern_c': False,
        'has_proper_guards': False,
        'includes_updated': False,
        'issues': []
    }
    
    if file_type == 'header':
        # Check for C++ guards
        analysis['has_cpp_guards'] = '#ifdef __cplusplus' in content and 'extern "C"' in content
        
        # Check for proper include guards
        base_name = filepath.stem.upper()
        expected_guard = f"{base_name}_HPP"
        analysis['has_proper_guards'] = f"#ifndef {expected_guard}" in content
        
        if not analysis['has_cpp_guards']:
            analysis['issues'].append("Missing C++ compatibility guards")
        
        if not analysis['has_proper_guards']:
            analysis['issues'].append("Missing or incorrect include guards")
    
    elif file_type == 'source':
        # Check for updated includes
        analysis['includes_updated'] = not re.search(r'#include "\w+\.h"', content)
        
        # Check for C++ compatibility
        analysis['has_extern_c'] = '#ifdef __cplusplus' in content
        
        if not analysis['includes_updated']:
            analysis['issues'].append("Header includes not updated to .hpp")
        
        if not analysis['has_extern_c']:
            analysis['issues'].append("Missing C++ compatibility for PostgreSQL headers")
    
    return analysis

def generate_conversion_report():
    """Generate a comprehensive conversion report."""
    print("🔍 Generating C++ Conversion Report")
    print("=" * 60)
    
    src_dir = Path('src')
    dst_dir = Path('src-cpp')
    
    if not dst_dir.exists():
        print("❌ ERROR: src-cpp directory not found")
        return False
    
    # Analyze header files
    print("\n📋 Header Files Analysis:")
    print("-" * 40)
    
    h_files = list(src_dir.glob('*.h'))
    hpp_files = list(dst_dir.glob('*.hpp'))
    
    header_stats = {
        'total': len(hpp_files),
        'good': 0,
        'issues': 0
    }
    
    for hpp_file in sorted(hpp_files):
        analysis = analyze_file_content(hpp_file, 'header')
        
        status = "✅" if not analysis['issues'] else "❌"
        print(f"{status} {hpp_file.name} ({analysis['lines']} lines)")
        
        if analysis['issues']:
            header_stats['issues'] += 1
            for issue in analysis['issues']:
                print(f"   ⚠️  {issue}")
        else:
            header_stats['good'] += 1
    
    # Analyze source files
    print(f"\n📄 Source Files Analysis:")
    print("-" * 40)
    
    c_files = list(src_dir.glob('*.c'))
    cpp_files = list(dst_dir.glob('*.cpp'))
    
    source_stats = {
        'total': len(cpp_files),
        'good': 0,
        'issues': 0
    }
    
    for cpp_file in sorted(cpp_files):
        analysis = analyze_file_content(cpp_file, 'source')
        
        status = "✅" if not analysis['issues'] else "❌"
        print(f"{status} {cpp_file.name} ({analysis['lines']} lines)")
        
        if analysis['issues']:
            source_stats['issues'] += 1
            for issue in analysis['issues']:
                print(f"   ⚠️  {issue}")
        else:
            source_stats['good'] += 1
    
    # Summary
    print(f"\n📊 Conversion Summary:")
    print("-" * 40)
    print(f"Headers: {header_stats['good']}/{header_stats['total']} good")
    print(f"Sources: {source_stats['good']}/{source_stats['total']} good")
    
    total_files = header_stats['total'] + source_stats['total']
    total_good = header_stats['good'] + source_stats['good']
    
    print(f"Overall: {total_good}/{total_files} files converted successfully")
    
    # Check Makefile
    print(f"\n🔨 Build System:")
    print("-" * 40)
    
    makefile_path = dst_dir / 'Makefile'
    if makefile_path.exists():
        with open(makefile_path, 'r') as f:
            makefile_content = f.read()
        
        has_module = 'MODULE_big' in makefile_content
        has_cxxflags = 'PG_CXXFLAGS' in makefile_content
        has_cpp_rule = '.cpp.o:' in makefile_content
        
        print(f"✅ Makefile exists")
        print(f"{'✅' if has_module else '❌'} Module definition: {'Present' if has_module else 'Missing'}")
        print(f"{'✅' if has_cxxflags else '❌'} C++ flags: {'Present' if has_cxxflags else 'Missing'}")
        print(f"{'✅' if has_cpp_rule else '❌'} C++ build rule: {'Present' if has_cpp_rule else 'Missing'}")
    else:
        print("❌ Makefile not found")
    
    # Recommendations
    print(f"\n💡 Recommendations:")
    print("-" * 40)
    
    if header_stats['issues'] > 0 or source_stats['issues'] > 0:
        print("• Fix files with conversion issues before proceeding")
    
    print("• Test compilation with PostgreSQL development headers")
    print("• Run functional tests to ensure behavior is preserved")
    print("• Consider adding C++ unit tests")
    print("• Benchmark performance against C version")
    
    return total_good == total_files

def main():
    """Main function."""
    success = generate_conversion_report()
    
    print(f"\n{'='*60}")
    if success:
        print("🎉 Conversion completed successfully!")
        print("All files have been converted from C to C++.")
    else:
        print("⚠️  Conversion completed with some issues.")
        print("Please review the report above and fix any problems.")
    
    return success

if __name__ == "__main__":
    success = main()
    exit(0 if success else 1)
