#pragma once
#include <array>
#include <condition_variable>
#include <format>
#include <mutex>
#include <iostream>
#include <string>
#include <string_view>
#include <format>

using namespace std::string_literals;

class CircularBuffer {


public:
	void put(int value) {
		{
			std::unique_lock dataLock(m_mutex);

			if (m_occupiedCells == m_buffer.size()) {
				std::cout << "Buffer is full. Producer waits.n \n\n";

				m_cv.wait(dataLock, [&] {return m_occupiedCells < m_buffer.size(); });
			}

			m_buffer[m_writeIndex] = value;
			++m_occupiedCells;
			m_writeIndex = (m_writeIndex + 1) % m_buffer.size();
			displayState(std::format("Procuder write {}", value));

		}
		m_cv.notify_one();
		

	}

	int get() {
		int readValue;
		{
			std::unique_lock dataLock(m_mutex);

			if (m_occupiedCells == 0) {
				std::cout << "Buffer is empty. Consumer waits \n\n";

				m_cv.wait(dataLock, [&]() {return m_occupiedCells > 0; });
			}

			readValue = m_buffer[m_readIndex];
			m_readIndex = (m_readIndex +1) % m_buffer.size();
			--m_occupiedCells;
			displayState(std::format("Consumer read {}", readValue));
			
		}
		m_cv.notify_one();
		return readValue;
	}



	void displayState(std::string_view operation) const {
		std::string s;

		s += std::format("{} (buffer cells occupied: {}) \n{:<15}",
			operation, m_occupiedCells, "buffer cells:");

		for (int value : m_buffer) {
			s += std::format(" {:2d}  ", value);
		}

		s += std::format("\n{:<15}", ""); //pading

		for (int i{ 0 }; i < m_buffer.size(); ++i) {
			s += "---- "s;
		}
		s += std::format("\n{:<15}", ""); //pading
		for (int i{ 0 }; i < m_buffer.size(); ++i) {
			s += std::format(" {}{}  ",
				(i == m_writeIndex ? 'W' : ' '),
				(i == m_readIndex ? 'R' : ' '));
	
		}
		s += "\n\n";
		std::cout << s;
		


	}
private:
	std::condition_variable m_cv;
	std::mutex m_mutex;

	std::array<int, 3> m_buffer{-1, -1, -1}; //shared m_buffer
	int m_occupiedCells{ 0 }; //count number of buffer used
	int m_writeIndex{ 0 };
	int m_readIndex{ 0 };
};