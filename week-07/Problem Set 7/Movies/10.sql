SELECT p.name FROM people p
JOIN directors d ON d.person_id = p.id
JOIN movies m ON m.id = d.movie_id
JOIN ratings r ON r.movie_id = m.id
WHERE r.rating >= 9.0