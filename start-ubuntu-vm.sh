#!/usr/bin/env bash
# start-ubuntu-vm.sh — Manage Ubuntu dev VM for kernel testing
# ------------------------------------------------------------
# First run boots from ISO to install Ubuntu.
# Later runs boot directly from the installed system.
#
# Options:
#   --snapshot   Run in snapshot (ephemeral) mode — no disk writes saved
#   --headless   Run without GUI (useful for SSH-only)
#   --help       Show this help message
# ------------------------------------------------------------

set -euo pipefail

# ===== CONFIGURATION =====
VM_NAME="ubuntu-dev"
QCOW2="$HOME/vmimages/ubuntu-dev.qcow2"
ISO="$HOME/isos/ubuntu-22.04.5-live-server-amd64.iso"
MEM="4096"      # MB
CPUS="2"
SSH_PORT="2222" # host port forwarded to guest 22
FLAG_FILE="${QCOW2}.installed"
# =========================

show_help() {
  cat <<EOF
Usage: $(basename "$0") [options]

Options:
  --snapshot      Run VM in snapshot mode (ephemeral, changes discarded)
  --headless      Run without GUI (text console only)
  --help          Show this help message and exit

Behavior:
  - On first run, boots from ISO to install Ubuntu.
  - After installation, the script automatically marks the VM as installed.
  - Next runs boot directly from disk.

Networking:
  SSH port forward: host port ${SSH_PORT} → guest port 22
  Connect from host:
        ssh -p ${SSH_PORT} <username>@localhost

EOF
  exit 0
}

SNAPSHOT=0
HEADLESS=0
for arg in "$@"; do
  case "$arg" in
    --snapshot) SNAPSHOT=1 ;;
    --headless) HEADLESS=1 ;;
    --help) show_help ;;
    *) echo "Unknown argument: $arg"; show_help ;;
  esac
done

# Ensure disk image exists
if [[ ! -f "$QCOW2" ]]; then
  echo "[*] Creating $QCOW2 (30G qcow2 image)"
  mkdir -p "$(dirname "$QCOW2")"
  qemu-img create -f qcow2 "$QCOW2" 30G
fi

# Build QEMU command
QEMU=(
  qemu-system-x86_64
  -enable-kvm
  -cpu host
  -smp "$CPUS"
  -m "$MEM"
  -drive "file=$QCOW2,format=qcow2,if=virtio"
  -netdev "user,id=net0,hostfwd=tcp::${SSH_PORT}-:22"
  -device virtio-net-pci,netdev=net0
  -machine accel=kvm
  -device virtio-balloon
)

# Display mode
if [[ $HEADLESS -eq 1 ]]; then
  QEMU+=(-nographic)
else
  QEMU+=(-display gtk)
fi

# Snapshot mode
[[ $SNAPSHOT -eq 1 ]] && QEMU+=(-snapshot)

# Boot mode
if [[ ! -f "$FLAG_FILE" ]]; then
  echo "[*] First run: booting installer from $ISO"
  QEMU+=(-cdrom "$ISO" -boot d)
  # automatically mark as installed so next time boots from disk
  touch "$FLAG_FILE"
else
  echo "[*] Booting installed system..."
  QEMU+=(-boot c)
fi

echo "------------------------------------------------------------"
echo "SSH port forward: host port ${SSH_PORT} → guest port 22"
echo "Use: ssh -p ${SSH_PORT} <username>@localhost"
[[ $SNAPSHOT -eq 1 ]] && echo "Running in SNAPSHOT mode (disk changes discarded)"
[[ $HEADLESS -eq 1 ]] && echo "Running in HEADLESS mode (no GUI)"
echo "------------------------------------------------------------"

exec "${QEMU[@]}"
