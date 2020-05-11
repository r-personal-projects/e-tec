file(REMOVE_RECURSE
  "hello-world.map"
  "dummy_main_src.c"
  "CMakeFiles/hello-world.elf.dir/dummy_main_src.c.obj"
  "hello-world.elf.pdb"
  "hello-world.elf"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/hello-world.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
