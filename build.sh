python3 -m venv "$APP_ROOT/venv"

PY="$APP_ROOT/venv/bin/python"

"$PY" -m pip install mkdocs-material

"$PY" -m mkdocs build -d "$APP_ROOT/site"
