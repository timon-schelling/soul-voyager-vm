plugins {
    id("cpp-application")
    id("visual-studio")
}

application {
    source.from(file("src"))
    privateHeaders.from(file("src"))
}

tasks.withType(CppCompile::class.java).configureEach {
    compilerArgs.add("-static")
    compilerArgs.add("-static-libgcc")
    compilerArgs.add("-static-libstdc++")
}
