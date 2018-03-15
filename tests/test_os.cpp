//
// Created by josem on 9/28/17.
//

//
// Created by Jose Miguel de la Rosa Trevin on 2017-03-24.
//

#include <iostream>
#include "gtest/gtest.h"

#include "em/os/filesystem.h"


using namespace em;


TEST(File, Basic) {
    // Test basic properties of Type singleton instances
    const char * filename = "kk.binary";

    FILE *f = fopen(filename, "w");
    float v[1024];
    memset(v, 0, 1024*sizeof(float)); // Set all to zero

    // Write 1024 floats
    size_t written = fwrite(v, sizeof(float), 1024, f);
    std::cout << "Written: " << written << " elements" << std::endl;

    fflush(f);

    ASSERT_EQ(Path::getFileSize(filename), 4*1024);

    File::resize(f, 5 * 1024);
    fclose(f);

    ASSERT_EQ(Path::exists(filename), true);
    ASSERT_EQ(Path::getFileSize(filename), 5*1024);

    ASSERT_EQ(Path::remove(filename), 0);
    ASSERT_FALSE(Path::exists(filename));

    std::string filename2("non-existing");
    // Check that removing a non-existing file is fine
    ASSERT_EQ(Path::remove(filename2), 0);
} // TEST File.Basic


TEST(Path, Basic) {
    // Test basic properties of Type singleton instances
    std::string fn1 = "path/to.from/there/kk.binary";
    std::string dn1 = Path::getDirName(fn1);
    std::string bn1 = Path::getFileName(fn1);

    ASSERT_EQ(dn1, "path/to.from/there");
    ASSERT_EQ(Path::getDirName(dn1), "path/to.from");
    ASSERT_EQ(Path::getDirName(fn1 + "/"), fn1);

    ASSERT_EQ(bn1, "kk.binary");
    ASSERT_EQ(Path::getFileName(bn1), bn1);
    ASSERT_EQ(Path::getFileName(fn1 + "/"), "");

    ASSERT_EQ(Path::getExtension(fn1), "binary");
    ASSERT_EQ(Path::getExtension(bn1), "binary");
    ASSERT_EQ(Path::getExtension("binary"), "");

    ASSERT_EQ(Path::getExtension("a.b.c"), "c");
} // TEST Path.Basic
