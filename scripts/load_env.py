import os

Import("env")  # PlatformIO/SCons construction env

def load_env_file(path):
    if not os.path.isfile(path):
        print(f"[load_env] No {path} found, skipping")
        return
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#") or "=" not in line:
                continue
            k, v = line.split("=", 1)
            k = k.strip()
            v = v.strip().strip('"').strip("'")
            os.environ.setdefault(k, v)

load_env_file(os.path.join(os.getcwd(), ".env"))

ip = os.environ.get("ESP_IP", "").strip()
print("[load_env] ESP_IP =", ip)

if not ip:
    print("[load_env] ERROR: ESP_IP missing/empty")
else:
    # This is what espota uses
    env.Replace(UPLOAD_PORT=ip)
    # optional: makes it visible elsewhere in PIO build scripts
    env.Replace(ESP_IP=ip)
