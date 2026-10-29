// Generated from app/app_status_page.html by app/app_embed_status_page.py. Do not edit.
#ifndef STATUS_PAGE_H
#define STATUS_PAGE_H
#include <Arduino.h>

static const char STATUS_PAGE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html><head><meta charset="utf-8"><title>JO Sun Tracker</title></head>
<body><h1>JO Sun Tracker</h1>
<p>Prochaine comparaison LDR dans : <strong id="countdown">--</strong> s</p>
<pre id="status">...</pre>
<h2>Logs</h2><pre id="log">...</pre>
<script>
let nextComparisonAt = null;
function updateCountdown() {
  const el = document.getElementById('countdown');
  if (nextComparisonAt === null) { el.textContent = '--'; return; }
  el.textContent = Math.max(0, Math.ceil((nextComparisonAt - Date.now()) / 1000));
}
function refresh() {
  fetch('/status').then(r => r.json()).then(d => {
    document.getElementById('status').textContent = JSON.stringify(d, null, 2);
    nextComparisonAt = Date.now() + d.status.nextComparisonMs;
    updateCountdown();
  });
}
function refreshLog() {
  fetch('/log').then(r => r.text()).then(t => {
    const el = document.getElementById('log');
    el.textContent = t;
    el.scrollTop = el.scrollHeight;
  });
}
refresh();
refreshLog();
setInterval(refresh, 1000);
setInterval(updateCountdown, 1000);
setInterval(refreshLog, 2000);
</script></body></html>
)HTML";

#endif
