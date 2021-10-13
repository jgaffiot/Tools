"""Unitary tests of BaseError.hh"""

import pytest

import PyTools

BaseError = PyTools.tools.BaseError


def test_base_error(tmp_path):
    """Unitary test of tools::BaseError"""
    with pytest.raises(BaseError) as exc_info:
        raise BaseError(0)
        assert exc_info.value == 0

    with pytest.raises(BaseError) as exc_info:
        raise BaseError("error: ", 0, " something failed: ", 42)
        assert exc_info.value == "error: 0 something failed: 42"
