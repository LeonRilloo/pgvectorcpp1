#include "src-cpp/core/vector.hpp"
#include <iostream>
#include <cassert>

void test_basic_operations() {
    std::cout << "Testing basic operations..." << std::endl;
    
    // Test construction
    Vector v1(3);
    v1[0] = 1.0f;
    v1[1] = 2.0f;
    v1[2] = 3.0f;
    
    Vector v2(3);
    v2[0] = 4.0f;
    v2[1] = 5.0f;
    v2[2] = 6.0f;
    
    // Test addition
    Vector v3 = v1 + v2;
    assert(v3[0] == 5.0f);
    assert(v3[1] == 7.0f);
    assert(v3[2] == 9.0f);
    
    // Test subtraction
    Vector v4 = v2 - v1;
    assert(v4[0] == 3.0f);
    assert(v4[1] == 3.0f);
    assert(v4[2] == 3.0f);
    
    // Test multiplication
    Vector v5 = v1 * v2;
    assert(v5[0] == 4.0f);
    assert(v5[1] == 10.0f);
    assert(v5[2] == 18.0f);
    
    // Test scalar multiplication
    Vector v6 = v1 * 2.0f;
    assert(v6[0] == 2.0f);
    assert(v6[1] == 4.0f);
    assert(v6[2] == 6.0f);
    
    std::cout << "Basic operations test passed!" << std::endl;
}

void test_norms() {
    std::cout << "Testing norms..." << std::endl;
    
    Vector v(3);
    v[0] = 3.0f;
    v[1] = 4.0f;
    v[2] = 0.0f;
    
    // Test L2 norm
    float l2 = v.l2_norm();
    assert(std::abs(l2 - 5.0f) < 1e-6f);
    
    // Test L1 norm
    float l1 = v.l1_norm();
    assert(std::abs(l1 - 7.0f) < 1e-6f);
    
    std::cout << "Norms test passed!" << std::endl;
}

void test_comparison() {
    std::cout << "Testing comparison operators..." << std::endl;
    
    Vector v1(2);
    v1[0] = 1.0f;
    v1[1] = 2.0f;
    
    Vector v2(2);
    v2[0] = 1.0f;
    v2[1] = 2.0f;
    
    Vector v3(2);
    v3[0] = 2.0f;
    v3[1] = 3.0f;
    
    assert(v1 == v2);
    assert(v1 != v3);
    assert(v1 < v3);
    assert(v3 > v1);
    assert(v1 <= v2);
    assert(v1 >= v2);
    
    std::cout << "Comparison test passed!" << std::endl;
}

void test_serialization() {
    std::cout << "Testing serialization..." << std::endl;
    
    Vector v1(3);
    v1[0] = 1.0f;
    v1[1] = 2.0f;
    v1[2] = 3.0f;
    
    // Serialize
    char buffer[1024];
    v1.serialize(buffer);
    
    // Deserialize
    Vector v2 = Vector::deserialize(buffer);
    
    assert(v1.getDim() == v2.getDim());
    assert(v1[0] == v2[0]);
    assert(v1[1] == v2[1]);
    assert(v1[2] == v2[2]);
    
    std::cout << "Serialization test passed!" << std::endl;
}

int main() {
    test_basic_operations();
    test_norms();
    test_comparison();
    test_serialization();
    
    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}
