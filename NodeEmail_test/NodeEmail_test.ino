#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// Wi-Fi credentials
const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

// Gmail SMTP server credentials
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define EMAIL_SENDER "sznvws@gmail.com"
#define EMAIL_PASSWORD "ilxl gvex wzct paxm"
#define EMAIL_RECIPIENT "reme25241@gmail.com"

// Create SMTP session and configuration
SMTPSession smtp;
Session_Config config;
SMTP_Message message;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Configure SMTP session
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = EMAIL_SENDER;
  config.login.password = EMAIL_PASSWORD;

  smtp.callback(smtpCallback); // Optional: Debug callback

  // Configure email message
  message.sender.name = "NodeMCU Email Bot";
  message.sender.email = EMAIL_SENDER;
  message.subject = "Hello from NodeMCU!";
  message.addRecipient("Recipient", EMAIL_RECIPIENT);
  message.text.content = "This is a test email sent from NodeMCU using Gmail SMTP server.";

  // Connect to SMTP server and send the email
  if (!smtp.connect(&config)) {
    Serial.printf("Failed to connect to SMTP server: %s\n", smtp.errorReason().c_str());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("Failed to send email: %s\n", smtp.errorReason().c_str());
  } else {
    Serial.println("Email sent successfully!");
  }

  smtp.closeSession(); // Close the session
}

void loop() {
  // Nothing to do in loop
}

// Optional: Callback function for debugging
void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());
}
