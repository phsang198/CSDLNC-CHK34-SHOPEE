package com.mtth.shopee.controller;

import com.mtth.shopee.model.SearchLogRequest;
import com.mtth.shopee.model.SearchLogResponse;
import com.mtth.shopee.service.SearchLogService;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/search")
@RequiredArgsConstructor
@Validated
public class SearchLogController {

    private final SearchLogService searchLogService;

   
    @PostMapping("/log")
    public String logSearch(@RequestBody @Valid SearchLogRequest request) {
        searchLogService.logSearch(request);
        return "Search log recorded successfully!";
    }

   
    @GetMapping("/logs")
    public List<SearchLogResponse> getAllLogs() {
        return searchLogService.getAllLogs();
    }

    
    @GetMapping("/logs/{userId}")
    public List<SearchLogResponse> getLogsByUserId(@PathVariable @NotBlank String userId) {
        return searchLogService.getLogsByUserId(userId);
    }
}