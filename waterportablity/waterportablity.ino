#include <OneWire.h>
#include <DallasTemperature.h>

// Pin assignments
#define ONE_WIRE_BUS 7  // DS18B20 data pin
#define TDS_SENSOR_PIN A0  // TDS sensor connected to analog pin A0
#define TURBIDITY_SENSOR_PIN A1  // Turbidity sensor connected to analog pin A1

// Initialize the OneWire instance and DallasTemperature library
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Variables for sensor readings
float temperature = 0.0;
int tdsValue = 0;
int turbidityValue = 0;
float tds = 0.0;
float salinity = 0.0;
float hardness = 0.0;
float pH = 7.0;  // Default value (you can add a pH sensor for accurate readings)

// Water Factors and Modifications by RO
struct WaterFactor {
  String factor;
  String modification;
};

// Array of water factors and corresponding RO modifications
WaterFactor waterFactors[] = {
  {"TDS (Total Dissolved Solids)", "Significant reduction (90-99%)"},
  {"Salinity", "Reduction of salt concentration"},
  {"Hardness", "Softening by removing calcium and magnesium"},
  {"Microorganisms/Pathogens", "Removal of bacteria, viruses, protozoa"},
  {"Chlorine/Chloramine", "Removal of chlorine and chloramine"},
  {"Heavy Metals", "Removal of lead, arsenic, mercury, copper, cadmium"},
  {"Nitrates/Nitrites", "Removal of harmful nitrogen compounds"},
  {"Organic Contaminants", "Removal of pesticides, herbicides, and chemicals"},
  {"Turbidity", "Reduction in cloudiness and suspended particles"},
  {"Taste and Odor", "Improve taste by removing contaminants"}
};

// Setup
void setup() {
  // Start the serial communication
  Serial.begin(9600);
  sensors.begin();  // Start the DS18B20 sensor
  randomSeed(analogRead(0));  // Seed the random number generator
}

// Loop to read sensors and display results
void loop() {
  // Generate random temperature between 0 and 50 °C (realistic range for water)
  temperature = random(28, 30);  // Random temperature from 0 to 50°C
  
  // Generate random TDS value between 0 and 1000 ppm (typical range for water quality)
  tds = random(40, 55);  // Random TDS value from 0 to 1000 ppm
  
  // Generate random turbidity value between 0 and 500 NTU (typical turbidity range)
  turbidityValue = random(45, 55);  // Random turbidity value from 0 to 500 NTU
  
  // Display all sensor values and categorize water quality
  Serial.println("Water Quality Analysis:");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("TDS Value: ");
  Serial.print(tds);
  Serial.println(" ppm");
  
  Serial.print("Turbidity Value: ");
  Serial.print(turbidityValue);
  Serial.println(" NTU");

  // Simulate other water quality checks
  salinity = tds * 0.5;  // Rough estimation for salinity
  hardness = tds * 0.3;  // Rough estimation for hardness
  
  if (salinity <= 500) {
    Serial.println("This water is deemed conducive to sustaining aquatic life and is generally safe for human consumption in moderate quantities.");
  } else {
    Serial.println("Excessive salinity can detrimentally affect plant growth, disrupt aquatic ecosystems, and render the water less suitable for human consumption.");
  }

  if (hardness <= 100) {
    Serial.println("Hardness is within normal range (soft water).");
  } else {
    Serial.println("Hardness is out of normal range (hard water).");
  }

  // Display random water factors and their modifications by RO
  int index1 = random(0, sizeof(waterFactors) / sizeof(waterFactors[0]));
  int index2 = random(0, sizeof(waterFactors) / sizeof(waterFactors[0]));

  // Ensure the two random indexes are different
  while (index2 == index1) {
    index2 = random(0, sizeof(waterFactors) / sizeof(waterFactors[0]));
  }

  // Display the randomly selected water factors and modifications
  Serial.println();
  Serial.print("Water Factor: ");
  Serial.print(waterFactors[index1].factor);
  Serial.print(" - ");
  Serial.println(waterFactors[index1].modification);

  Serial.print("Water Factor: ");
  Serial.print(waterFactors[index2].factor);
  Serial.print(" - ");
  Serial.println(waterFactors[index2].modification);

  // Delay before the next reading
  delay(5000);
} 
