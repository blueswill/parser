#ifndef DEBUG
#define DEBUG
#endif
#include"cfg_graph.hpp"
#include"cfg_reduction.hpp"
#include<iostream>

int main () {
    parser::cfg_builder builder;
    std::cin >> builder;
    //std::cout << std::string(builder.get_cfg()) << std::endl;
    std::cout << std::string(builder.get_cfg().get_LR_graph().get_reduction_table());
    return 0;
}
