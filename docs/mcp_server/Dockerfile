FROM python:3.12-slim

WORKDIR /app

COPY docs/mcp_server/requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Bake docs, headers, and demos into the image
COPY docs/ /app/docs/
COPY include/ /app/include/
COPY demos/ /app/demos/
COPY docs/mcp_server/server.py .

ENV DOCS_DIR=/app/docs
ENV INCLUDE_DIR=/app/include
ENV DEMOS_DIR=/app/demos
ENV TRANSPORT_MODE=stdio
ENV TRANSPORT_HOST=0.0.0.0
ENV TRANSPORT_PORT=8080

EXPOSE 8080

HEALTHCHECK --interval=30s --timeout=5s --retries=3 \
    CMD python -c "import urllib.request; urllib.request.urlopen('http://localhost:${TRANSPORT_PORT}/health')" || exit 0

ENTRYPOINT ["python", "server.py"]
