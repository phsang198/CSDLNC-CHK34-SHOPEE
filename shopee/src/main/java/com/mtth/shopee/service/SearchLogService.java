package com.mtth.shopee.service;

import com.datastax.oss.driver.api.core.CqlSession;
import com.datastax.oss.driver.api.core.cql.*;
import com.mtth.shopee.model.SearchLogRequest;
import com.mtth.shopee.model.SearchLogResponse;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.net.InetSocketAddress;
import java.time.Instant;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

@Service
@RequiredArgsConstructor
public class SearchLogService {

    private final CqlSession session;
    private final PreparedStatement insertStatement;
    private final PreparedStatement selectAllStatement;
    private final PreparedStatement selectByUserIdStatement;

    public SearchLogService() {
        this.session = CqlSession.builder()
            .addContactPoint(new InetSocketAddress("localhost", 9042))
            .withLocalDatacenter("datacenter1")
            .withKeyspace("shopee")
            .build();

        this.insertStatement = session.prepare(
            "INSERT INTO search_logs_by_user_date " +
            "(user_id, search_date, timestamp, search_query, device, location, category) " +
            "VALUES (?, ?, ?, ?, ?, ?, ?)"
        );

        this.selectAllStatement = session.prepare(
            "SELECT * FROM search_logs_by_user_date ALLOW FILTERING"
        );

        this.selectByUserIdStatement = session.prepare(
            "SELECT * FROM search_logs_by_user_date WHERE user_id = ? ALLOW FILTERING"
        );
    }

    

    public void logSearch(SearchLogRequest request) {
        BoundStatement bound = insertStatement.bind(
            request.getUserId(),
            request.getSearchDate(),
            Instant.now(),
            request.getSearchQuery(),
            request.getDevice(),
            request.getLocation(),
            request.getCategory()
        );

        session.execute(bound);
    }

    public List<SearchLogResponse> getAllLogs() {
        ResultSet resultSet = session.execute(selectAllStatement.bind());
        return mapResultSet(resultSet);
    }

    public List<SearchLogResponse> getLogsByUserId(String userId) {
        BoundStatement bound = selectByUserIdStatement.bind(UUID.fromString(userId));
        ResultSet resultSet = session.execute(bound);
        return mapResultSet(resultSet);
    }

    private List<SearchLogResponse> mapResultSet(ResultSet resultSet) {
        List<SearchLogResponse> responses = new ArrayList<>();
        for (Row row : resultSet) {
            responses.add(new SearchLogResponse(
                row.getUuid("user_id").toString(),
                row.getLocalDate("search_date").toString(),
                row.getString("search_query"),
                row.getString("device"),
                row.getString("location"),
                row.getString("category")
            ));
        }
        return responses;
    }

    public List<String> getTop5PopularKeywords() {
        PreparedStatement selectPopularKeywords = session.prepare(
            "SELECT search_query FROM popular_keywords_by_date WHERE search_date = ? LIMIT 5"
        );
    
        BoundStatement bound = selectPopularKeywords.bind(LocalDate.now());
        ResultSet resultSet = session.execute(bound);
    
        List<String> popularKeywords = new ArrayList<>();
        for (Row row : resultSet) {
            popularKeywords.add(row.getString("search_query"));
        }
    
        return popularKeywords;
    } 
}
