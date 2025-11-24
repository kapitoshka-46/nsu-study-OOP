#include <gtest/gtest.h>
#include <fstream>
#include "../src/core/Core.h"
#include "../src/file-handler/file_handler.h"
#include <filesystem>

namespace fs = std::filesystem;
const std::string pwd = "../../";
TEST(Save, Glider) {

    std::ifstream in {pwd + "examples/glider.lif"};
    core::Universe u1;
    core::Universe u2;

    file_handler::LoadFromFile(in, u1);
    std::ofstream out(pwd + "test/saveload/saved_glider.lif");

    file_handler::SaveToFile(out, u1);
    out.close();
    in.close();
    in.open(pwd + "test/saveload/saved_glider.lif");
    file_handler::LoadFromFile(in, u2);

    ASSERT_EQ(u1.Rows(), u2.Rows());
    ASSERT_EQ(u1.Cols(), u2.Cols());

    const auto &f1 = u1.GetField();
    const auto &f2 = u1.GetField();

    for (int i = 0; i < u1.Rows(); i++) {
        for (int j = 0; j < u2.Cols(); j++) {
            ASSERT_EQ(f1.GetCellState({i, j}), f2.GetCellState({i, j}));
        }
    }

}

TEST(Save, Pauchek) {

    std::ifstream in {"../../examples/pauchek.lif"};
    core::Universe u1;
    core::Universe u2;

    file_handler::LoadFromFile(in, u1);
    std::ofstream out("../../test/saveload/saved_pauchek.lif");

    file_handler::SaveToFile(out, u1);
    out.close();
    in.close();
    in.open("../../test/saveload/saved_pauchek.lif");
    file_handler::LoadFromFile(in, u2);

    ASSERT_EQ(u1.Rows(), u2.Rows());
    ASSERT_EQ(u1.Cols(), u2.Cols());

    const auto &f1 = u1.GetField();
    const auto &f2 = u1.GetField();

    for (int i = 0; i < u1.Rows(); i++) {
        for (int j = 0; j < u2.Cols(); j++) {
            ASSERT_EQ(f1.GetCellState({i, j}), f2.GetCellState({i, j}));
        }
    }

}