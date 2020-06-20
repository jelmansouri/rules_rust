def _impl(ctx):
    args = ctx.actions.args()
    args.add("--rustc", ctx.rustc)
    args.add("--output", ctx.outputs.out)
    args.add("--")
    args.add_all(ctx.args)
    ctx.actions.run(
        executable = ctx.executable._bin,
        arguments = [args],
        env = env,
        inputs = [ctx.inputs],
        outputs = [ctx.outputs.out],
        mnemonic = "Rustc",
    )

expanded_template = rule(
    implementation = _impl,
    attrs = {
        "rustc": attr.output(mandatory = True),
        "rename_from": attr.string(),
        "rename_to": attr.string(),
        "inputs": attr.string_list(
            allow_single_file = True,
            mandatory = True,
        ),
        "outputs": attr.string_list(
            allow_single_file = True,
            mandatory = True,
        ),
        "args": attr.string_list(
            allow_single_file = True,
            mandatory = True,
        ),
        "env": attr.string_dict(),
        "_bin": attr.label(
            default = "//rust/private/rustc_wrapper:rustc_wrapper",
            executable = True,
            allow_single_file = True,
            cfg = "host",
        ),
    },
)
