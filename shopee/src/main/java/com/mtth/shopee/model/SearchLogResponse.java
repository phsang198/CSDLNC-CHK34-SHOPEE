package com.mtth.shopee.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class SearchLogResponse {
    private String userId;
    private String searchDate;
    private String searchQuery;
    private String device;
    private String location;
    private String category;
}
