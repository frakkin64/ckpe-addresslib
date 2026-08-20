target("ckpe-addresslibgen")
    -- set build by default
    set_default(false)

    -- set build group
    set_group("tools")

    -- add source files
    add_files("src/**.cpp")
