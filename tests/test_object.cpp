//
// Created by Jose Miguel de la Rosa Trevin on 2017-03-24.
//

#include <iostream>
#include "gtest/gtest.h"

#include "em/base/error.h"
#include "em/base/object.h"
#include "em/base/array.h"
#include "em/image/image.h"
#include "em/base/timer.h"


using namespace em;


TEST(Object, Basic)
{
    // Empty object constructor
    Object eo;
    // After this, the object type should be Null
    ASSERT_EQ(eo.getType(), typeNull);
    ASSERT_TRUE(eo.getType().isNull());

    eo = 1; // Now this object should change its type to typeInt32
    ASSERT_EQ(eo.getType(), typeInt32);
    // When the object type is not null, it should not
    // change its type and just try to convert to the underlying type.
    eo = 2.2f;
    ASSERT_EQ(eo.getType(), typeInt32);
    ASSERT_EQ((int)eo, (int)2.2f);

    // Copy constructor
    em::Object o(1);
    int x = o;
    ASSERT_EQ(o.getType(), em::typeInt32);
    ASSERT_EQ(x, 1);
    o = 2;
    ASSERT_EQ(int(o), 2);

    em::Object o2(3.5); // Type should be double
    ASSERT_EQ(o2.getType(), em::typeDouble);
    // We can explicitly change the type of a given object
    o2.setType(typeFloat);
    ASSERT_EQ(o2.getType(), typeFloat);
    o2 = 1.3; // Still the type should be float and not double
    ASSERT_EQ(o2.getType(), typeFloat);
    float f = o2;
    ASSERT_FLOAT_EQ(f, 1.3f);

    o2 = 5.6f;

    float d, d2 = 5.6 + float(o2);
    float d3;

    size_t N = 100;
    float values [] = {1.5f, 2.3f, 5.7f, 3.2f, 10.f, 56.f};
    std::vector<Object> vobj;

    for (int i = 0; i < N; i++)
    {
        d = values[i % 6];
        vobj.push_back(Object(d));
    }

    for (int i = 0; i < N; ++i)
    {
        d = vobj[i];
        ASSERT_FLOAT_EQ(d, values[i % 6]);
    }

    const char * str = "This is a test string";

    Object o3;
    o3 = std::string(str);
    std::string s2 = o3;
    ASSERT_EQ(s2, str);

    Image img(ArrayDim(10, 10), typeFloat);
    auto& typeImage = Type::get<Image>();
    o3.setType(typeImage);
    o3 = img;
    ASSERT_EQ(o3.getType(), typeImage);
    Image img2 = o3;

    // Test copy and equality
    Object oo3 = std::string("One string");
    Object oo2 = oo3;
    ASSERT_EQ(oo2, oo3);

    Object o4(oo3);
    ASSERT_EQ(o4, oo3);

    ObjectVector ov;
    ov.resize(3);

    ov[0] = oo3;
    ASSERT_EQ(ov[0], oo3);
    ASSERT_EQ(ov[0], oo2);
    //ASSERT_EQ((std::string) o2, (std::string) o3);
    Object o5(oo2);
    ASSERT_EQ(oo2, o5);
} // TEST Object.Basic

TEST(Object, Parsing)
{
    // Test the parsing methods
    Object o1 = 0.0, o2 = 1.0;
    std::string pi("3.14159");
    std::stringstream ss(pi);
    o1.fromStream(ss);
    o2.fromString(pi);
    ASSERT_FLOAT_EQ((double)o1, 3.14159);
    ASSERT_FLOAT_EQ((double)o2, 3.14159);
    ASSERT_EQ(o1.toString(), pi);
    // FIXME: The following Object comparision throws an exception
    // ASSERT_FLOAT_EQ(o1, o2);
    std::cout << o1.toString() << std::endl;
} // TEST Object.Parsing