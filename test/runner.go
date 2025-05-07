package main

import (
	"bufio"
	"fmt"
	"net/http"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"
)

type Data struct {
	host     string
	port     int
	testNbr  int
	timeout  int
	runTime  int // if >0, run for this many seconds instead of fixed count
}

func main() {
	// defaults
	cfg := Data{"127.0.0.1", 8080, 10, 10, 0}
	reader := bufio.NewReader(os.Stdin)

	fmt.Printf("Enter host (default %s): ", cfg.host)
	if input, _ := reader.ReadString('\n'); strings.TrimSpace(input) != "" {
		cfg.host = strings.TrimSpace(input)
	}

	fmt.Printf("Enter port (default %d): ", cfg.port)
	if input, _ := reader.ReadString('\n'); strings.TrimSpace(input) != "" {
		if p, err := strconv.Atoi(strings.TrimSpace(input)); err == nil {
			cfg.port = p
		} else {
			fmt.Fprintf(os.Stderr, "Invalid port, using default %d\n", cfg.port)
		}
	}

	fmt.Printf("Enter number of requests (default %d): ", cfg.testNbr)
	if input, _ := reader.ReadString('\n'); strings.TrimSpace(input) != "" {
		if n, err := strconv.Atoi(strings.TrimSpace(input)); err == nil {
			cfg.testNbr = n
		} else {
			fmt.Fprintf(os.Stderr, "Invalid count, using default %d\n", cfg.testNbr)
		}
	}

	fmt.Printf("Enter timeout in seconds (default %d): ", cfg.timeout)
	if input, _ := reader.ReadString('\n'); strings.TrimSpace(input) != "" {
		if t, err := strconv.Atoi(strings.TrimSpace(input)); err == nil {
			cfg.timeout = t
		} else {
			fmt.Fprintf(os.Stderr, "Invalid timeout, using default %d\n", cfg.timeout)
		}
	}

	fmt.Printf("Enter running time in seconds (default %d; 0=disabled): ", cfg.runTime)
	if input, _ := reader.ReadString('\n'); strings.TrimSpace(input) != "" {
		if rt, err := strconv.Atoi(strings.TrimSpace(input)); err == nil {
			cfg.runTime = rt
		} else {
			fmt.Fprintf(os.Stderr, "Invalid duration, using default %d\n", cfg.runTime)
		}
	}

	runTest(cfg)
}

func runTest(d Data) {
	url := fmt.Sprintf("http://%s:%d/", d.host, d.port)
	mode := "fixed count"
	if d.runTime > 0 {
		mode = fmt.Sprintf("duration %ds", d.runTime)
	}
	fmt.Printf("\nTesting %s with %s, timeout %ds...\n\n", url, mode, d.timeout)

	client := http.Client{Timeout: time.Duration(d.timeout) * time.Second}
	var (
		wg         sync.WaitGroup
		mu         sync.Mutex
		successes  int
		failures   int
		totalReqs  int
	)

	start := time.Now()

	if d.runTime > 0 {
		// time-based: loop until elapsed >= runTime
		for time.Since(start) < time.Duration(d.runTime)*time.Second {
			wg.Add(1)
			go func() {
				defer wg.Done()
				resp, err := client.Get(url)
				mu.Lock()
				defer mu.Unlock()
				totalReqs++
				if err != nil {
					failures++
					return
				}
				defer resp.Body.Close()
				if resp.StatusCode >= 200 && resp.StatusCode < 300 {
					successes++
				} else {
					failures++
				}
			}()
		}
	} else {
		// count-based: fire exactly testNbr concurrent requests
		for i := 0; i < d.testNbr; i++ {
			wg.Add(1)
			go func() {
				defer wg.Done()
				resp, err := client.Get(url)
				mu.Lock()
				defer mu.Unlock()
				totalReqs++
				if err != nil {
					failures++
					return
				}
				defer resp.Body.Close()
				if resp.StatusCode >= 200 && resp.StatusCode < 300 {
					successes++
				} else {
					failures++
				}
			}()
		}
	}

	wg.Wait()
	elapsed := time.Since(start)

	fmt.Println("----- Results -----")
	fmt.Printf("Total requests sent:   %d\n", totalReqs)
	fmt.Printf("Successful (2xx):      %d\n", successes)
	fmt.Printf("Failed (error/other):  %d\n", failures)
	fmt.Printf("Total time elapsed:    %v\n", elapsed)
	fmt.Printf("Requests per second:   %.2f\n", float64(totalReqs)/elapsed.Seconds())
}
