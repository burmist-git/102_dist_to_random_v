# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & run

Requires CERN ROOT; the Makefile calls `$(ROOTSYS)/bin/root-config`, so `ROOTSYS` must be set (e.g. `source <root>/bin/thisroot.sh`).

```bash
make            # builds obj/sim.o and the ./runsim binary
make clean      # removes runsim, obj/, editor backups

./runsim 0 conf.conf <nsteps> <output.root>   # simulation (runID 0)
./runsim 100 conf.conf                         # test: read + print the config
./runsim 101 conf.conf <out.root>              # test: sierpinskiStr polygon checks, writes gr_polygon
./runsim.sh -d                                 # = ./runsim 0 conf.conf 30000 trajectories.root
./runsim.sh -t                                 # = ./runsim 100 conf.conf
./runsim.sh -ts                                # = ./runsim 101 conf.conf test_sierpinski.root
root -l plots.C                                # plot gr_frame + gr_tr_<i> graphs from ./trajectories.root
```

There are no tests or linters.

## Architecture

- `runsim.cpp` — CLI entry point. Dispatches on `argv[1]` (runID): 0 = simulation, 100 = config-read test, 101 = `test_sierpinski()` (prints PASS/FAIL, exits 1 on failure). Reads config into `simConfStr`, builds a `sim` with a fixed-seed `TRandom3` (seed 12313), then calls `runsim(nsteps)`, `save_graph(file)`, `save_gif("tr_gif", 20)`.
- `src/simConfStr.hh` — header-only config struct. `readFromFile` scans whitespace-separated tokens for `key:` and reads the next value. Adding a config parameter means: add the member, its default in the constructor, a line in `printInfo`, and a `mot == "key:"` branch in `readFromFile`. Unknown keys and `#` comments in `.conf` files are silently ignored.
- `src/sierpinskiStr.hh` — header-only regular polygon (`ver_n` vertices, side `side_size`) built from `simConfStr`; centered at (0,0), bottom edge horizontal.
- `src/sim.{hh,cpp}` — simulation class. `sim` owns (and deletes) the `TRandom3` and `simConfStr` passed to it.
- Makefile dependencies are hand-written: if you add a new source/header in `src/`, add an object rule and link it into `runsim`.

## Current state (important)

This repo was forked from an earlier 2D N-body/trajectory simulation and stripped down:
- `sim::runsim` is an empty stub.
- `save_graph` and `save_gif` bodies are commented out; they reference removed pieces (`_bodyState_v` / `bodyStateStr.hh`, `frame_x0`, `frame_y0`, `frame_H`) and must be rewritten against the new config (`n`, `ver_n`, `side_size`, `ratio_s`/`ratio`, `frame_L`, `frame_margine`) before re-enabling.
- `runsim.sh` modes other than `-d` and `-t` (`-p`, `-g`) call runIDs 1, 2 that no longer exist in `runsim.cpp`, and reference CSV inputs that were deleted.
- Output conventions from the old code (to keep `plots.C` working): ROOT file contains `TGraph`s named `gr_frame`, `gr_tr_<i>` (trajectory), `gr_v_<i>` (velocity); GIF frames go to `tr_gif/gr_<k>.gif` plus a generated `tr_gif/mrg.sh` that merges them with `gifsicle`.
