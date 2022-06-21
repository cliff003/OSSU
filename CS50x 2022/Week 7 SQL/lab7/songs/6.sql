SELECT s.name
FROM songs s
LEFT JOIN artists a
ON s.artist_id = a.id
WHERE a.name = "Post Malone";