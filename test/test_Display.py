"""Unitary tests of Display.hh"""

import cppyy
import PyTools

std = cppyy.gbl.std

BaseError = PyTools.tools.BaseError

type_name = PyTools.tools.type_name


class TestClass:
    pass


def test_type_name():
    """Unitary test of tools::type_name"""
    assert type_name[None]() == "void"
    assert type_name[int]() == "int"
    assert type_name[str]() == "std::basic_string<char>"
    assert type_name[std.vector[int]]() == "std::vector<int, std::allocator<int> >"
    assert type_name[cppyy.nullptr]() == "void"


def test_operator_stream_enum(capfd):
    """Unitary test of tools::operator<<(ostream, const Enum&)"""
    cppyy.include("Display.hh")
    cppyy.cppdef(
        """
        using tools::operator<<;
        enum class TestEnum{ a, b=42};
        void test_enum_stream(TestEnum val) { std::cout << val << std::endl; };
    """
    )
    cppyy.gbl.test_enum_stream(cppyy.gbl.TestEnum.a)
    out, _ = capfd.readouterr()
    assert out == "0\n"

    cppyy.gbl.test_enum_stream(cppyy.gbl.TestEnum.b)
    out, _ = capfd.readouterr()
    assert out == "42\n"
