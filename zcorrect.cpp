#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char *agrv[])
{
    std::string fname = "./ramka_Engraving.ngc";
    std::fstream ngc_file(fname.c_str());

    if (!ngc_file.is_open())
    {
        std::cerr << "failed to open " << fname << '\n';
        return EXIT_FAILURE;
    }

    // search pattern like
    // G0 Z10.
    // X195.1891 Y5.1638
    // G1 Z-0.3 F200.
    std::string l1_sample = "G0 Z10.\r";
    std::string l1, l2, l3;

    uint32_t line_num {1};
    while (std::getline(ngc_file, l1))
    {
        ++line_num;

    //    std::cout << l1 << std::endl;
        if (l1 != l1_sample)
            continue;
        // cache the rest of a block
        if (!std::getline(ngc_file, l2) || !std::getline(ngc_file, l3))
        {
            std::cerr << "block is not complete, exiting...\n";
            break;
        }

        line_num += 2;

        std::cout << l2 << std::endl;
        // parse xyz coordinates
        auto x_pos = l2.find('X');
        if (x_pos == std::string::npos)
        {
            std::cerr << line_num-1 << ": doesn't include 'X' symbol" <<
                " skipping block\n";
            continue;
        }

        try
        {
            l2.erase(x_pos, 1);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what();
            std::cerr << "x_pos: " << x_pos << std::endl;
        }

        auto y_pos = l2.find('Y');
        if (y_pos == std::string::npos)
        {
            std::cerr << line_num-1 << ": doesn't include 'Y' symbol" <<
                " skipping block\n";
            continue;
        }

        try
        {
            l2.erase(y_pos, 1);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what();
            std::cerr << " y_pos: " << y_pos << std::endl;
        }

        std::stringstream ss2(l2);
        double x{0};
        double y{0};
        ss2 >> x >> y;

        auto z_pos = l3.find("Z", 0);
        auto ll3 = l3.substr(z_pos, l3.length() - z_pos);
        std::stringstream ss3(ll3);
        double z{0};
        ss3 >> z;
        std::cout << "X: " << x << " Y: " << y
            << " Z: " << z << std::endl;
    }

}
