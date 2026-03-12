# robo_op_sys_clt_lib

A compact C-based ROS 2 helper library and example node that demonstrates minimal publisher/subscriber and service/client usage using `rcl` and `ament_cmake`. In addition to the ROS primitives, this project provides a JSON-based serialization layer (via `cJSON`) and an `iface` abstraction so your ROS messages can be expressed as JSON and transported over non-ROS transports. This enables interoperability with web services, cloud endpoints, web dashboards, and non-DDS systems.

This README focuses on how the JSON interface works and how to integrate the library with systems outside of ROS / DDS.

---

## Key features

- Minimal example executable (`node`) demonstrating publisher, subscriber, service, and client in plain C.
- `iface` abstraction for serializing/deserializing C data structures into JSON.
- Bundled `cJSON` headers for parsing/printing JSON.
- Enables bridging ROS topics/services to HTTP/REST/WebSocket/MQTT or any UTF-8-capable transport.
- Small, dependency-light codebase intended for embedded or resource-constrained systems.

---

## Why JSON interoperability?

- JSON is ubiquitous: every major language and platform can parse and produce JSON.
- Human-readable: makes debugging and quick integration (e.g., curl, Postman) trivial.
- Works across firewalls and cloud endpoints (HTTP/S, WebSockets) without requiring DDS or a ROS bridge.
- Great for dashboards, mobile apps, cloud telemetry ingestion, or integrating third-party services.

---

## Where the JSON code lives

The project includes a trimmed `cJSON` header and an `iface` layer that you can use to convert between C structs and JSON documents. See the bundled header for the JSON API surface:

```robo_op_sys_clt_lib/include/robo_op_sys_clt_lib/cjson.h#L1-40
#ifndef ROBO_OP_SYS_CLT_LIB_CJSON_H
#define ROBO_OP_SYS_CLT_LIB_CJSON_H

#ifdef __cplusplus
extern "C"
{
#endif
```

(Full header is installed under `include/robo_op_sys_clt_lib/`.)

---

## How the `iface` / JSON layer works (conceptually)

1. Define your C data structure (fields, types, offsets).
2. Create an `interface_type` (a lightweight descriptor that lists fields and their types).
3. Use the library to `serialize_interface()` to produce an `interface_t` representing the message in memory.
4. Convert the `interface_t` to JSON (string) using the `cJSON` helpers and print/send it on whatever transport you choose.
5. On the receiving end, parse the JSON back into an `interface_t` (or a direct C struct) using `cJSON`, validate it, and then consume it either locally (as a ROS message) or forward it into the ROS graph.

This approach decouples message content from transport. You're free to send JSON over DDS (as a string field), HTTP, WebSockets, TCP, MQTT, etc.

---

## Example serialized payload

For the simple example in `main.c`, the message has one numeric field `count`. The JSON you will produce/consume looks like:

```/dev/null/example-payload.json#L1-1
{"count": 42}
```

---

## Example: forward a ROS topic to an external HTTP endpoint

If you want to bridge `/count` messages to an HTTP REST endpoint, serialize the message to JSON and POST it.

Example curl request the bridge might issue (or you can test with curl directly):

```/dev/null/example-curl.sh#L1-3
curl -X POST -H "Content-Type: application/json" \
  -d '{"count": 42}' \
  https://example.com/telemetry
```

In practice you would implement a small bridge component (C, Python, Node.js, etc.) that subscribes to the ROS topic, serializes each message to JSON using the same field names, and pushes them to your endpoint.

---

## Envelope pattern (recommended for production integrations)

When bridging to non-ROS systems, it's often useful to include metadata in an envelope so receivers can route and validate messages:

```/dev/null/example-envelope.json#L1-7
{
  "topic": "/count",
  "type": "custom_msgs/Count",
  "seq": 1234,
  "ts": 1670000000.123,
  "payload": { "count": 42 }
}
```

Benefits:
- Receiver knows the source topic and message type.
- Helpful for routing messages in centralized collectors or brokers.
- Easier to replay, debug, and audit messages.

---

## Integration patterns

- HTTP/REST bridge
  - Node subscribes to a topic, serializes JSON, and POSTs to a REST API.
  - Use TLS and authentication for production.
- WebSocket live updates
  - Bridge publishes JSON messages upstream; browsers can display live state.
- MQTT telemetry
  - Publish JSON messages under topic namespaces (e.g., `robot/<id>/count`).
- Direct socket or TCP
  - Send newline-delimited JSON (NDJSON) for stream parsing on the server.
- Hybrid architectures
  - Keep DDS/ROS for closed-loop control and use JSON for telemetry, logging, or web UI.

---

## Security and robustness recommendations

- Always authenticate and authorize external endpoints before sending data.
- Use TLS (HTTPS / WSS / MQTT over TLS) for transport encryption.
- Validate incoming JSON carefully. Enforce field types and ranges.
- Protect against denial-of-service by rate-limiting and bounding parse complexity.
- Consider signing or adding HMAC to messages if you require end-to-end authenticity.
- If you need better performance or compactness, consider binary encodings (CBOR, protobuf). JSON is ideal for interoperability and readability but not always the highest-performance choice.

---

## Implementation notes and best practices

- Keep the `iface` schema stable for external consumers. Add version or schema fields to envelopes when changing shapes.
- Field names used in JSON should be consistent and documented; prefer snake_case or camelCase and stay consistent across components.
- When bridging services, serialize both requests and responses to JSON, and include correlation IDs so clients can match replies.
- For long-lived streams prefer NDJSON or a framing protocol to avoid ambiguity when parsing streams over sockets.

---

## Build & run (quick)

Place this package inside a ROS 2 workspace `src/` and build:

```/dev/null/build-steps.sh#L1-6
# from your ROS 2 workspace root
cd ~/ros2_ws

# build the package
colcon build --packages-select robo_op_sys_clt_lib

# source the workspace after build
. install/setup.bash
```

Run the example node:

```/dev/null/run-node.sh#L1-3
# source your workspace (if not already)
. install/setup.bash

# run the node
ros2 run robo_op_sys_clt_lib node
```

---

## Extending and binding to other transports

- Write a small bridge module that subscribes to the topic(s) you want, uses the `iface` JSON serialization, and publishes that JSON to the transport of your choice.
- For languages other than C, implement the same JSON schema and parsing logic; you do not need the ROS client libraries on the other side.
- For very small or embedded devices, implement minimal JSON serialization compatible with the `iface` schema and send to your bridge for reinjection into ROS if needed.

---

## Development & contributions

- Headers live under `include/robo_op_sys_clt_lib/`.
- Sources live under `src/`.
- Update `CMakeLists.txt` when you add sources.
- Prefer explicit error checking in the serialization/deserialization path.
- If you add fields or types, document the JSON schema and consider adding an example JSON schema or JSON Schema file for consumers.

---

## License

This project is released under the MIT License. See the `LICENSE` file included with the repository for the full text.

---

If you want, I can:
- Add example bridge code (C, Python, or Node.js) that subscribes to a topic, serializes JSON, and POSTs to an HTTP endpoint.
- Add a sample WebSocket server and browser demo that consumes the JSON payload.
- Produce a small JSON Schema file describing the message shapes to help external consumers validate incoming messages.

Tell me which example bridge or language you'd like and I'll provide a concrete snippet.