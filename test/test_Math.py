"""Unitary tests of Math.hh"""

import PyTools


def test_constant():
    """Unitary test of some constnant of tools::Math."""
    assert PyTools.tools.Math.Pi() == 3.141592653589793


def test_square():
    """Unitary test of tools::Math::sq(uare)"""
    m = PyTools.tools.Math
    assert m.sq(0) == 0
    assert m.sq(1) == 1
    assert m.sq(2) == 4
    assert m.sq(1000) == 1000000
    assert m.square(10) == 100


def test_cube():
    """Unitary test of tools::Math::cube"""
    m = PyTools.tools.Math
    assert m.cube(0) == 0
    assert m.cube(1) == 1
    assert m.cube(2) == 8
    assert m.cube(10) == 1000


def test_pow_n():
    """Unitary test of tools::Math::pow_n"""
    m = PyTools.tools.Math
    assert m.pow_n(0, 0) == 1
    assert m.pow_n(42, 0) == 1
    assert m.pow_n(0, 1) == 0
    assert m.pow_n(0, 42) == 0
    assert m.pow_n(1, 1) == 1
    assert m.pow_n(1, 42) == 1
    assert m.pow_n(2, 0) == 1
    assert m.pow_n(2, 1) == 2
    assert m.pow_n(2, 2) == 4
    assert m.pow_n(2, 3) == 8
    assert m.pow_n(2, 10) == 1024
    assert m.pow_n(10, 6) == 1000000


def test_sum2():
    """Unitary test of tools::Math::sum2"""
    m = PyTools.tools.Math
    assert m.sum2(0, 0) == 0
    assert m.sum2(0, 10) == 100
    assert m.sum2(10, 0) == 100
    assert m.sum2(1, 1) == 2
    assert m.sum2(2, 2) == 8
    assert m.sum2(10, 10) == 200
    assert m.sum2(10, 1) == 101


def test_norm2():
    """Unitary test of tools::Math::norm2"""
    m = PyTools.tools.Math
    assert m.norm2(0, 0) == 0
    assert m.norm2(0, 10) == 10
    assert m.norm2(10, 0) == 10
    assert m.norm2(1, 1) == m.Sqrt2()
    assert m.norm2(3, 4) == 5


def test_diff2():
    """Unitary test of tools::Math::diff2"""
    m = PyTools.tools.Math
    assert m.diff2(0, 0) == 0
    assert m.diff2(42, 42) == 0
    assert m.diff2(10, 0) == 100
    assert m.diff2(0, 10) == -100
    assert m.diff2(1, 2) == -3
    assert m.diff2(1, 10) == -99
    assert m.diff2(4, 5) == -9
