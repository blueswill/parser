#include"cfg_graph.hpp"
#include<iostream>

int main () {
    parser::cfg_builder builder;
    std::cin >> builder;
    auto cfg = builder.get_cfg();
    //std::cout << std::string(cfg) << std::endl;
    auto graph = cfg.get_LR_graph();
    std::cout << std::string(graph) << std::endl;
    parser::LR_graph::Data data;
    std::cout << (graph.is_conflict(&data) ? "LR(1) is conflict" : "LR(1) is not conflict")
        << std::endl;
    std::cout << std::string(data) << std::endl;
    return 0;
}
