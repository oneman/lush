curl -i -X PUT -T add_jack_adapter.json localhost:5000/xpdr/Jack
curl -i -X PUT -T source.json localhost:5000/xpdr/Jack/Music
curl -i -X PUT -T output.json localhost:5000/xpdr/Jack/Main
curl -i -X PUT -T mixer_input.json localhost:5000/mixer/Main/Music

curl -i localhost:5000/xpdr/Jack
curl -i localhost:5000/xpdr/Jack/Music
curl -i localhost:5000/mixer/Music
curl -i localhost:5000/xpdr/Jack/Main
curl -i localhost:5000/mixer/Main
curl -i localhost:5000/mixer/Main/Music
