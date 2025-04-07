package com.mtth.shopee.model;

// Assuming this is the existing class structure
public class SearchLogRequest {
    private String userId;
    private String searchDate;
    private String searchQuery;
    private String device;
    private String location;
    private String category;

    // Constructor to match the parameters used in the controller
    public SearchLogRequest(String userId, String searchDate, String searchQuery, String device, String location, String category) {
        this.userId = userId;
        this.searchDate = searchDate;
        this.searchQuery = searchQuery;
        this.device = device;
        this.location = location;
        this.category = category;
    }

    // Getters and setters (if not already present)
    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public String getSearchDate() {
        return searchDate;
    }

    public void setSearchDate(String searchDate) {
        this.searchDate = searchDate;
    }

    public String getSearchQuery() {
        return searchQuery;
    }

    public void setSearchQuery(String searchQuery) {
        this.searchQuery = searchQuery;
    }

    public String getDevice() {
        return device;
    }

    public void setDevice(String device) {
        this.device = device;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }
}