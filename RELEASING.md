# Releasing

Releases are tagged `vX.Y.Z` on `main` and published as GitHub releases. The C manifests (`CMakeLists.txt`,
`library.json`, `library.properties`, `idf_component.yml`) and the Rust crates share the same version, so bump them
all together before tagging. The Rust crates.io flow will be added here together with the bindings.

1. Update the version in `CMakeLists.txt` (`project(... VERSION X.Y.Z)`), `library.json`, `library.properties` and
   `idf_component.yml`.
2. Commit, push, and wait for CI to pass on `main`.
3. Create the tag and the release:

   ```sh
   git tag vX.Y.Z
   git push origin vX.Y.Z
   gh release create vX.Y.Z --generate-notes
   ```

## PlatformIO

Publishing a GitHub release runs `.github/workflows/publish-platformio.yml`, which checks that the `library.json`
version matches the tag (without the leading `v`), packs the library with `pio pkg pack` and publishes it to the
[PlatformIO registry](https://registry.platformio.org/libraries/alixanneraud/lvgl_theme_shadcn) under the
`alixanneraud` owner.

The workflow needs a `PLATFORMIO_AUTH_TOKEN` repository secret:

1. Log in with `pio account login`, then create a token with `pio account token`.
2. Add it under **Settings > Secrets and variables > Actions** as `PLATFORMIO_AUTH_TOKEN`, or with
   `gh secret set PLATFORMIO_AUTH_TOKEN`.

The workflow can also be started by hand from the Actions tab (`workflow_dispatch`); the version check is skipped in
that case, and the registry rejects a version that already exists.
