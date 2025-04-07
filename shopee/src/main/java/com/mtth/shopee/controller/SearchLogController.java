package com.mtth.shopee.controller;

import com.datastax.oss.driver.api.core.CqlSession;
import com.datastax.oss.driver.api.core.cql.*;
import com.mtth.shopee.model.SearchLogRequest;
import org.springframework.web.bind.annotation.*;

import java.net.InetSocketAddress;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

@RestController
@RequestMapping("/api/search")
public class SearchLogController {

    private final CqlSession session;
    private final PreparedStatement insertStatement;
    private final PreparedStatement selectAllStatement;
    private final PreparedStatement selectByUserIdStatement;

    public SearchLogController() {
        // Kết nối Cassandra
        this.session = CqlSession.builder()
        .addContactPoint(new InetSocketAddress("localhost", 9042))
        .withLocalDatacenter("datacenter1")
        .withKeyspace("shopee")
        .build();

        // Prepare insert statement
        this.insertStatement = session.prepare("INSERT INTO search_logs_by_user_date " +
        "(user_id, search_date, timestamp, search_query, device, location, category) " +
        "VALUES (?, ?, ?, ?, ?, ?, ?)");

        // Prepare select all statement WITH ALLOW FILTERING 
        this.selectAllStatement = session.prepare("SELECT * FROM search_logs_by_user_date ALLOW FILTERING");

        // Prepare select by user_id statement WITH ALLOW FILTERING 
        this.selectByUserIdStatement = session.prepare(
            "SELECT * FROM search_logs_by_user_date WHERE user_id = ? ALLOW FILTERING"
        );
        
    }

    @PostMapping("/log")
    public String logSearch(@RequestBody SearchLogRequest request) {
        BoundStatement bound = insertStatement.bind(
                UUID.fromString(request.getUserId()),
                request.getSearchDate(), // Để nguyên là String vì cột search_date trong Cassandra là text
                Instant.now(),
                request.getSearchQuery(),
                request.getDevice(),
                request.getLocation(),
                request.getCategory()
        );

        session.execute(bound);
        return "Search log recorded successfully!";
    }

    @GetMapping("/logs")
    public List<SearchLogRequest> getAllLogs() {
        List<SearchLogRequest> logs = new ArrayList<>();
        ResultSet resultSet = session.execute(selectAllStatement.bind());

        for (Row row : resultSet) {
            logs.add(mapRowToRequest(row));
        }

        return logs;
    }

    @GetMapping("/logs/{userId}")
    public List<SearchLogRequest> getLogsByUserId(@PathVariable String userId) {
        List<SearchLogRequest> logs = new ArrayList<>();
        BoundStatement bound = selectByUserIdStatement.bind(UUID.fromString(userId));
        ResultSet resultSet = session.execute(bound);

        for (Row row : resultSet) {
            logs.add(mapRowToRequest(row));
        }

        return logs;
    }

    // Helper function to map Row -> SearchLogRequest
    private SearchLogRequest mapRowToRequest(Row row) {
        return new SearchLogRequest(
                row.getUuid("user_id").toString(),
                row.getLocalDate("search_date").toString(),
                row.getString("search_query"),
                row.getString("device"),
                row.getString("location"),
                row.getString("category")
        );
    }
}
