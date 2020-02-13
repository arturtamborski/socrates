#!/usr/bin/env bash

FPS=24

STREAM="https://videos3.earthcam.com/fecnetwork"
STREAM="$STREAM/AbbeyRoadHD1.flv/chunklist_w1632720834.m3u8"

STREAM="./stream.mp4"


args=(
    # ---------------------- source --- #
    -i "$STREAM"

    # --------------------- logging --- #
    #-loglevel warning

    # ----------------------- scale --- #
    #-s "1920:1080"         # 100% of FHD
    #-s "1600:900"          #  80% of FHD
    #-s "1280:720"          #  66% of FHD
     -s "960:540"           #  50% of FHD

    # ----------------- frame limit --- #
     -vf "fps=$FPS"

    # ------------ overwrite output --- #
    -y

    # -------------- show timestamp --- #
    #-vf "drawtext=$drawtext"

    # ---------- remove audio track --- #
    -an

    # ------------------------ fast --- #
    -preset ultrafast
    -tune zerolatency

    # --------------- sync fps diff --- #
    -vsync drop

    # --------------- output format --- #
    #-f image2
    #-f image2pipe
     -f image2 -update 1

    # ---------------- limit frames --- #
    -vframes 10

    # -------------- post to server --- #
    #http://127.0.0.1:8080/%d
    tcp://127.0.0.1:2563
)


FFMPEG="$(command -v ffmpeg)"

echo "$FFMPEG" "${args[@]}"
exec "$FFMPEG" "${args[@]}"
