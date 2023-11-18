#!/bin/sh

OPS_SERVER="localhost:5000"
TASK_FILE="/tmp/random.task"
KR_JSON_TOOL="$KRAD_BUILD_DIR/tools/kr_json"

$KR_JSON_TOOL -r task > $TASK_FILE
echo "Sending:"
cat $TASK_FILE
echo ""
curl -i -s -T /tmp/random.task -X POST $OPS_SERVER/task
echo ""
rm $TASK_FILE
echo ""
curl $OPS_SERVER/tasks
echo ""
