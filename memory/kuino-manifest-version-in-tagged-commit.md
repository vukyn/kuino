---
name: kuino-manifest-version-in-tagged-commit
description: "kuino version bump — set library.json version IN the commit you tag, else PlatformIO shows stale version+sha"
metadata: 
  node_type: memory
  type: feedback
---

When bumping kuino (or any git-URL PlatformIO lib), the `library.json` `version` field MUST be edited in the SAME commit that gets the git tag. Bumping the tag alone leaves the manifest stale → PlatformIO's dependency graph shows the OLD version + `+sha.<commit>` (e.g. `kuino @ 0.1.0+sha.c21f735` for a v0.2.0 tag). The `+sha` is the true signal, but the label misleads.

**Why:** consumers pin the git tag (`kuino.git#vX.Y.Z`) so the tag is source of truth, but PlatformIO reads the manifest `version` for its label. Bump-then-tag as separate steps always leaves manifest one step behind the tag.

**How to apply:** workflow per [[kuery-shared-lib-rule]] scoped to kuino — edit module + bump `library.json` version to `X.Y.Z` in ONE commit → PR → merge → `git tag -a vX.Y.Z` on the merge commit → push → repin consumers. Verify `git show vX.Y.Z:library.json | grep version` == tag. NEVER re-point/delete a published tag (auto-mode blocks it; cut a new patch tag instead). kuino has modules wifi/httpjson/display/button/mp3 (mp3=YX5300/YX6300 driver: write cmds + v0.3.0 read API query/poll — queryFileCount 0x48, queryCurrentIndex 0x4C, queryStatus 0x42, EVT_ SD-in/out+track-finished). Latest tag: **v0.3.0** (manifest bumped in-commit, correct). **5 tags now (v0.1.0,0.2.0,0.2.1,0.2.2,0.3.0) = at retention limit → prune oldest (v0.1.0) local+remote on next bump.** See [[iot-trust-pio-ci-not-clang]].
