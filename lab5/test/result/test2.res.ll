; ModuleID = 'test2.res'
source_filename = "../../../docker_dir/test/test2.ll"

@a = global i32 0
@sm_main = global i32 0

define void @.init.a() {
  store i32 1, i32* @a
  ret void
}

define i32 @f(i32 %x) {
  %.0 = load i32, i32* @a
  %.1 = mul i32 2, %.0
  %.2 = add i32 %x, %.1
  ret i32 %.2
}

define void @.init.sm_main() {
  %.0 = call i32 @f(i32 5)
  store i32 %.0, i32* @sm_main
  ret void
}

define void @sm_init() {
  call void @.init.a()
  call void @.init.sm_main()
  ret void
}
