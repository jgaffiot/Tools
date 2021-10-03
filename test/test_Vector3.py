"""Unitary tests of Vector3"""

from math import pi

import PyTools

Vector3 = PyTools.tools.Vector3


def test_Vector3():
    """Unitary test of tools::Vector3"""
    v0 = Vector3()
    assert v0 == Vector3()
    assert Vector3(1, 1, 1) + Vector3(2, 2, 2) == Vector3(3, 3, 3)

    assert v0.x() == 0
    assert v0.y() == 0
    assert v0.z() == 0

    v1 = Vector3(1, 2, 3)
    for i in range(3):
        assert v1(i) == i + 1
        assert v1[i] == i + 1
        assert v1.at(i) == i + 1

    v2 = Vector3(0, 3, 4)
    assert v2.Norm() == 5
    assert v2.Mag() == 5
    assert v2.Mag2() == 25

    v2.SetX(1)
    v2.SetY(2)
    v2.SetZ(3)
    assert v2 == v1

    v2.SetXYZ(0, 0, 0)
    assert v2 == v0

    v2.SetMagThetaPhi(1, 0.0, 0.0)
    assert v2.Mag() == 1
    assert v2 == Vector3(0.0, 0.0, 1.0)

    v2.SetMagThetaPhi(1, pi / 2.0, pi / 2.0)
    assert v2.Mag() == 1
    assert v2 == Vector3(0.0, 1.0, 0.0)

    v2.SetPtThetaPhi(1, 0.0, 0.0)
    assert v2.Mag() == 1
    assert v2 == Vector3(1.0, 0.0, 0.0)

    v2.SetRhoPhiZ(1, pi / 2.0, 1)
    assert v2 == Vector3(0.0, 1.0, 1.0)
