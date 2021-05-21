"""Unitary tests of Vector3"""

import PyTools

Vector3 = PyTools.tools.Vector3


def test_Vector3():
    """Unitary test of tools::Vector3"""
    v = Vector3()
    assert v.x() == 0
    assert v.y() == 0
    assert v.z() == 0

    v = Vector3(1, 2, 3)
    for i in range(3):
        assert v(i) == i + 1
        assert v[i] == i + 1
        assert v.at(i) == i + 1

    v = Vector3(0, 3, 4)
    assert v.Norm() == 5
    assert v.Mag() == 5
    assert v.Mag2() == 25

    assert (Vector3(1, 2, 3) + Vector3(-1, 1, 1)).Mag() == v.Mag()
