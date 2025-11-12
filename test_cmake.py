import subprocess
import os
import sys

def run_command(cmd, cwd=None):
    """Run a command and return its output"""
    try:
        result = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"Command failed: {cmd}")
            print(f"Error: {result.stderr}")
            return False
        return True
    except Exception as e:
        print(f"Error running command: {e}")
        return False

def main():
    # Create build directory
    build_dir = "./build"
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Test CMake configuration
    print("Testing CMake configuration...")
    
    # Configure with CMake
    if not run_command("cmake ../src-cpp", cwd=build_dir):
        print("CMake configuration failed!")
        return False
    
    # Try to build
    if not run_command("make", cwd=build_dir):
        print("Build failed!")
        return False
    
    print("CMake configuration and build successful!")
    return True

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
