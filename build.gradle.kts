plugins {
    id("cpp-application")
    id("visual-studio")
}

application {
    source.from(file("src"))
    privateHeaders.from(file("src"))
}
