"""Unitary tests of Math.hh"""

import PyTools


def test_constant():
    """Unitary test of some constnant of tools::Math."""
    assert PyTools.tools.Math.Pi() == 3.141592653589793


def test_square():
    """Unitary test of tools::Math::sq(uare)"""
    sq = PyTools.tools.Math.sq
    square = PyTools.tools.Math.square
    assert sq(0) == 0
    assert sq(1) == 1
    assert sq(2) == 4
    assert sq(1000) == 1000000
    assert square(10) == 100


def test_cube():
    """Unitary test of tools::Math::cube"""
    cube = PyTools.tools.Math.cube
    assert cube(0) == 0
    assert cube(1) == 1
    assert cube(2) == 8
    assert cube(10) == 1000


def test_pow_n():
    """Unitary test of tools::Math::pow_n"""
    pow_n = PyTools.tools.Math.pow_n
    assert pow_n(0, 0) == 1
    assert pow_n(42, 0) == 1
    assert pow_n(0, 1) == 0
    assert pow_n(0, 42) == 0
    assert pow_n(1, 1) == 1
    assert pow_n(1, 42) == 1
    assert pow_n(2, 0) == 1
    assert pow_n(2, 1) == 2
    assert pow_n(2, 2) == 4
    assert pow_n(2, 3) == 8
    assert pow_n(2, 10) == 1024
    assert pow_n(10, 6) == 1000000


def test_sum2():
    """Unitary test of tools::Math::sum2"""
    sum2 = PyTools.tools.Math.sum2
    assert sum2(0, 0) == 0
    assert sum2(0, 10) == 100
    assert sum2(10, 0) == 100
    assert sum2(1, 1) == 2
    assert sum2(2, 2) == 8
    assert sum2(10, 10) == 200
    assert sum2(10, 1) == 101


def test_norm2():
    """Unitary test of tools::Math::norm2"""
    norm2 = PyTools.tools.Math.norm2
    assert norm2(0, 0) == 0
    assert norm2(0, 10) == 10
    assert norm2(10, 0) == 10
    assert norm2(1, 1) == PyTools.tools.Math.Sqrt2()
    assert norm2(3, 4) == 5


def test_diff2():
    """Unitary test of tools::Math::diff2"""
    diff2 = PyTools.tools.Math.diff2
    assert diff2(0, 0) == 0
    assert diff2(42, 42) == 0
    assert diff2(10, 0) == 100
    assert diff2(0, 10) == -100
    assert diff2(1, 2) == -3
    assert diff2(1, 10) == -99
    assert diff2(4, 5) == -9
