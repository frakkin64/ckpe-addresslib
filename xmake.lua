-- set minimum xmake version
set_xmakever("2.8.2")

-- set project
set_project("ckpe")
set_languages("c++23")
set_warnings("allextra")
set_encodings("utf-8")

-- add rules
add_rules("mode.debug", "mode.releasedbg")

-- require packages
add_requires("rsm-mmio")

-- include subprojects
includes("addresslibdecoder")
includes("addresslibgen")
