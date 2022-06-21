SELECT AVG(s.energy) AS avg_energy
FROM songs s
LEFT JOIN artists a
ON s.artist_id = a.id
GROUP BY s.artist_id
HAVING a.name = "Drake";